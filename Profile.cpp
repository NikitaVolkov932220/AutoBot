#include "Profile.h"
uProfile::uProfile() {
	uPower = 0;
	uID = 0;
	uPremium = false;
	uSubscribe = false;
}
uProfile::~uProfile() {

}
int uProfile::getPower() {
	return uPower;
}
int uProfile::getID() {
	return uID;
}


bool uProfile::getPremiumStatus() {
	return uPremium;
}
bool uProfile::getSubscribeStatus() {
	return uSubscribe;
}
bool uProfile::Start(int x) {
	bool res = true;
	wstring path = L"C:/Program Files/BlueStacks_nxt/HD-Player.exe";
	wstring cmd = (boost::wformat(LR"("%1%" --instance %2% --cmd launchApp --package %3%)") % path % (wstring(L"Pie64_") + to_wstring(x)) % "com.my.hc.rpg.kingdom.simulator").str();
	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if (!CreateProcessW(NULL, LPWSTR(cmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) res = false;
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return res;
}
bool uProfile::Initialize(int power, int ID, bool statusP, bool statusS) {
	uPower = power;
	uID = ID;
	uPremium = statusP;
	uSubscribe = statusS;
	if (uPower == 0) return false;
	return true;
}

void uProfile::Print() {
	cout << "User ID: " << uID << " Historical Power: " << uPower << " Premium status: ";
	if (uPremium == true) cout << "Yes ";
	else cout << "No ";
	cout << "Subscribe status: ";
	if (uSubscribe == true) cout << "Yes" << endl;
	else cout << "No" << endl;
}
void uProfile::setPower(int power) {
	uPower = power;

}
void uProfile::setID(int ID) {
	uID = ID;
}
void uProfile::setPremiumStatus(bool status) {
	uPremium = status;
}
void uProfile::setSubscribeStatus(bool status) {
	uSubscribe = status;
}

uProfile& uProfile::operator= (const uProfile& user) {
	this->uID = user.uID;
	this->uPower = user.uPower;
	this->uPremium = user.uPremium;
	this->uSubscribe = user.uSubscribe;
	return *this;
}
int squads::count = 0;