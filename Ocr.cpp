#include "Ocr.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <fstream>
#include <random>

namespace
{
	const Size gTemplateSymbolSize = { 20, 30 }; // mean: 16, 21 
};
namespace
{
	inline vector<int> GlyphsCount(size_t number)
	{
		vector<int> out;
		string data = to_string(number);
		for (auto c : data)
			if (c >= '0' && c <= '9')
				out.push_back(c - '0');
		return out;
	}
}

Ocr::Ocr(const path& unknown, const path& test, const path& train, const path& digits)
	: mUnknownDir(unknown), mTestDir(test), mTrainDir(train), mDigitsDir(digits)
{}

void Ocr::SetParams(const Params& settings)
{
	mParams = settings;
}
void Ocr::Initialize()
{
	Train();
	mIsLoaded = true;
}
void Ocr::Train()
{
	array<vector<Mat>, 10> glyphs;

	for (auto& it : directory_iterator(mTrainDir))
		if (it.path().has_extension() && it.path().extension() == ".png")
		{
			const string file_name = it.path().filename().replace_extension().string();
			if (!file_name.empty() && file_name[0] >= '0' && file_name[0] <= '9')
			{
				glyphs[file_name[0] - '0'].emplace_back(imread((it.path()).generic_string(), IMREAD_GRAYSCALE));
			}
			else
			{
				// TODO: warning
			}
		}

	if (false)
	{
		Size mean_size;
		int glyphs_count = 0;
		for (const auto& v : glyphs)
			for (const auto& m : v)
			{
				mean_size += m.size();
				++glyphs_count;
			}
		if (glyphs_count)
			mean_size = mean_size /= glyphs_count;
	}

	Mat array_chars;
	Mat array_images;
	for (size_t w = 0; w < glyphs.size(); ++w)
		for (const auto& img : glyphs[w])
		{
			Mat template_img;
			resize(img, template_img, gTemplateSymbolSize);
			Mat template_img_float;
			template_img.convertTo(template_img_float, CV_32FC1);
			Mat template_img_reshape = template_img_float.reshape(1, 1);

			array_images.push_back(template_img_reshape);
			array_chars.push_back('0' + static_cast<int>(w));
		}

	scoped_lock lock(mKnearesMutex);
	if (!array_images.empty())
	{
		mKNearest = ml::KNearest::create();
		mKNearest->setDefaultK(1);
		if (!mKNearest->train(array_images, ml::ROW_SAMPLE, array_chars))
		{
			// TODO: warning
		}
	}
}
void Ocr::LoadKnown()
{
	mLoadThread = jthread([this](stop_token stoken)
		{
			for (auto& it : directory_iterator(mTestDir))
				if (it.path().has_extension() && it.path().extension() == ".png")
				{
					if (stoken.stop_requested())
						return;

					const auto& path = it.path();
					size_t size = file_size(path);

					int number = 0;
					{
						auto txt_path = path;
						txt_path.replace_extension("txt");
						ifstream stream(txt_path, ifstream::binary);
						if (!stream)
						{
							// TODO: logw
							continue;
						}
						string buffer;
						stream.seekg(0, stream.end);
						buffer.resize(stream.tellg());
						stream.seekg(0, stream.beg);
						stream.read(buffer.data(), buffer.size());
						stream.close();
						if (buffer.empty())
						{
							// TODO: logw
							continue;
						}
						number = stoi(buffer);
					}

					TestUnit unit{ path, size, imread(path.generic_string()), number };

					if (mTestUnits.find(unit) == mTestUnits.end())
						mTestUnits.emplace(unit);
					else
						remove(path);
				}

			mIsLoaded = true;
		});
}

const Ocr::Params& Ocr::GetParams() const
{
	return mParams;
}

int Ocr::Recognize(const Mat& img) const
{
	if (img.empty())
		return -1;
	auto preprocessed = Preprocess(img);
	auto glyphs = FindGlyphs(preprocessed);

	if (glyphs.empty())
		return -1;

	int number = 0;
	for (size_t i = 0; i < glyphs.size(); ++i)
	{
		auto digit = RecognizeDigit(Mat(preprocessed, glyphs[glyphs.size() - i - 1]));
		if (digit < 0 || digit > 9)
			return -1;
		int power = static_cast<int>(pow(10, i));
		number += digit * power;
	}
	return number;
}
int Ocr::RecognizeDigit(const Mat& img) const
{
	if (img.empty())
		return false;

	Mat template_img;
	resize(img, template_img, gTemplateSymbolSize);
	Mat template_img_float;
	template_img.convertTo(template_img_float, CV_32FC1);
	Mat template_img_reshape = template_img_float.reshape(1, 1);

	float res = -1;
	{
		scoped_lock lock(mKnearesMutex);
		if (!mKNearest)
			return -1;
		try
		{
			Mat finded_symbol_img(0, 0, CV_32F);
			res = mKNearest->findNearest(template_img_reshape, 1, finded_symbol_img);
		}
		catch (const exception&)
		{
			// TODO: warning
			return -1;
		}
	}

	auto symbol = static_cast<char>(res);

	if (symbol >= '0' && symbol <= '9')
		return static_cast<int>(symbol - '0');

	return -1;
}

bool Ocr::IsLoaded() const
{
	return mIsLoaded;
}

Mat Ocr::DrawRects(const Mat& img, const vector<Rect>& rects) const
{
	Mat out;
	img.copyTo(out);
	for (const auto& rect : rects)
		rectangle(out, rect, Scalar(255, 0, 0), FILLED);
	return out;
}
Mat Ocr::Preprocess(const Mat& img) const
{
	/*imshow("1", img);
	waitKey(0);*/

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	//imshow("1", gray);
	//waitKey(0);

	Mat filter;
	inRange(gray, mParams.gray_threshold, 255, filter);
	//imshow("1", filter);
	//waitKey(0);

	Mat out;
	bitwise_and(gray, gray, out, filter);
	//imshow("1", out);
	//waitKey(0);

	return out;
}

vector<Rect> Ocr::FindGlyphs(const Mat& img) const
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<Rect> out;
	for (const auto& contour : contours)
	{
		auto brect = boundingRect(contour);
		if (brect.size().width < mParams.glyph_min_width)
			continue;
		if (brect.size().height < mParams.glyph_min_height)
			continue;
		if (brect.area() < mParams.glyph_min_area)
			continue;

		out.push_back(brect);
	}

	sort(out.begin(), out.end(), [](const Rect& a, const Rect& b)
		{
			return (a.br() + a.tl()).x / 2 < (b.br() + b.tl()).x / 2;
		});
	return out;
}

//////////////////////////
pair<path, Mat> Ocr::GetUnknownImage() const
{
	if (!mIsLoaded)
		return {};
	// Бежим по файлам, сравниваем со всеми тестовыми.

	for (auto& it : directory_iterator(mUnknownDir))
		if (it.path().has_extension() && it.path().extension() == ".png")
		{
			const auto& path = it.path();
			size_t size = file_size(path);
			TestUnit unit{ path, size, imread(path.generic_string()), 0 };
			if (mTestUnits.find(unit) == mTestUnits.end())
			{
				return { path, unit.img };
			}
			else
				remove(path);
		}

	return {};
}

void Ocr::AddKnownImage(const path& path, int number)
{
	if (!mIsLoaded)
		return;

	if (!exists(path))
		return;

	size_t size = file_size(path);
	TestUnit unit{ path, size, imread(path.generic_string()), 0 };
	if (mTestUnits.find(unit) == mTestUnits.end())
	{
		const auto new_path = mTestDir / path.filename();
		rename(path, new_path);
		unit.path = new_path;
		auto txt_path = new_path;
		txt_path.replace_extension("txt");
		{
			ofstream file;
			file.open(txt_path);
			file << number;
		}
		unit.number = number;
		mTestUnits.emplace(unit);
	}
	else
		remove(path);
}

Mat Ocr::Generate(const Mat& img) const
{
	Mat out = Mat::ones(img.size(), CV_8UC3);

	if (img.empty())
		return out;

	auto preprocessed = Preprocess(img);
	auto glyphs = FindGlyphs(preprocessed);

	if (glyphs.empty())
		return out;

	for (size_t i = 0; i < glyphs.size(); ++i)
	{
		const auto& glyph = glyphs[glyphs.size() - i - 1];
		auto digit = RecognizeDigit(Mat(preprocessed, glyph));
		if (digit < 0 || digit > 9)
		{
			out = Mat::ones(img.size(), CV_8UC3);
			return out;
		}

		Mat scaled;
		resize(mDigitsMat[digit], scaled, glyph.size());
		scaled.copyTo(Mat(out, glyph));
	}

	return out;
}

void Ocr::AddTrain(const TestUnit& unit)
{
	if (!mIsLoaded)
		return;

	auto src_glyphs = GlyphsCount(unit.number);
	auto preprocessed = Preprocess(unit.img);
	auto glyphs = FindGlyphs(preprocessed);
	if (glyphs.size() != src_glyphs.size())
	{
		// TODO: error
		return;
	}

	for (size_t i = 0; i < glyphs.size(); ++i)
	{
		const auto digit_img = Mat(preprocessed, glyphs[i]);
		auto digit = RecognizeDigit(digit_img);
		if (digit < 0 || digit != src_glyphs[i])
		{
			vector<uchar> buf;
			imencode(".png", digit_img, buf);
			const string_view digit_img_data(reinterpret_cast<const char*>(buf.data()), buf.size());
			const size_t suffix = hash<string_view>()(digit_img_data);

			const auto path = mTrainDir / (to_string(src_glyphs[i]) + "_" + to_string(suffix) + ".png");
			if (exists(path))
			{
				// TODO: warning
				continue;
			}

			if (!path.empty())
			{
				imwrite(path.generic_string(), digit_img);
				break;
			}
		}
	}

	Train();
}

optional<Ocr::TestUnit> Ocr::GetFailedTest()
{
	if (!mIsLoaded)
		return { {} };

	for (const auto& unit : mTestUnits)
		if (Recognize(unit.img) != unit.number)
			return unit;

	return {};
}

void Ocr::LoadDigits()
{
	for (size_t i = 0; i <= 9; ++i)
	{
		const auto path = mDigitsDir / (to_string(i) + ".png");
		if (exists(path))
			mDigitsMat[i] = imread(path.generic_string());
	}
}