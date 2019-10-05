#include<iostream>
#include"windows.h"

int main() {
	ShellExecute(NULL, TEXT("Open"), TEXT("launchv2.exe"), NULL, TEXT("Bin"), SW_HIDE);
	return 0;
}