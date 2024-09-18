#pragma once
#include <vector>
#include <filesystem>
#include <utility>
#include <atomic>
#include <thread>
#include <mutex>
#include <opencv2/ml.hpp>
#include <opencv2/core/mat.hpp>
#include <set>
#include <optional>
using namespace std;
using namespace cv;
using namespace filesystem;
class Ocr
{
public:
	Ocr(const path& unknown, const path& test, const path& train, const path& digits);
	~Ocr() = default;

	struct Params
	{
		int gray_threshold = 242;
		int glyph_min_width = 7;
		int glyph_min_height = 13;
		int glyph_min_area = 90;

		friend auto operator<=> (const Params& a, const Params& b) = default;
	};
	struct TestUnit
	{
		path path;
		size_t file_size = 0;
		Mat img;
		int number = 0;
	};

	Ocr(const Ocr& other) = delete;
	Ocr(Ocr&& other) noexcept = delete;
	Ocr& operator=(const Ocr& other) = delete;
	Ocr& operator=(Ocr&& other) noexcept = delete;

	void SetParams(const Params& settings);
	void Initialize();

	const Params& GetParams() const;

	bool IsLoaded() const;

	int Recognize(const Mat& img) const;
	int RecognizeDigit(const Mat& img) const;

	Mat Preprocess(const Mat& img) const;
	Mat DrawRects(const Mat& img, const vector<Rect>& rects) const;

	vector<Rect> FindGlyphs(const Mat& img) const;

	//////////////////////////////////////////////////////////////////
	Mat Generate(const Mat& img) const;
	pair<path, Mat> GetUnknownImage() const;
	void AddKnownImage(const path& path, int number);
	void AddTrain(const TestUnit& unit);
	optional<TestUnit> GetFailedTest();
	void LoadDigits();

private:
	void LoadKnown();
	void Train();

	struct TestUnitCmp
	{
		bool operator()(const TestUnit& a, const TestUnit& b) const
		{
			auto a_size = a.img.rows * a.img.cols * a.img.elemSize1();
			auto b_size = b.img.rows * b.img.cols * b.img.elemSize1();

			return (a.file_size < b.file_size) ||
				((!(b.file_size < a.file_size)) && (string_view(reinterpret_cast<const char*>(a.img.data), a_size) < string_view(reinterpret_cast<const char*>(b.img.data), b_size)));
		}
	};

	mutable mutex mKnearesMutex;
	Ptr<ml::KNearest> mKNearest;

	const path mUnknownDir;
	const path mTestDir;
	const path mTrainDir;
	const path mDigitsDir;

	set<TestUnit, TestUnitCmp> mTestUnits;

	array<Mat, 10> mDigitsMat;

	jthread mLoadThread;
	atomic<bool> mIsLoaded = false;

	Params mParams;
};

