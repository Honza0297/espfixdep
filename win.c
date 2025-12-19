/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "utils.h"
#include <io.h>
#include <windows.h>

char* EOL = "\r\n";

int run_process(char* argv[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD exitCode = 0;
	char* cmdl;
	char* cmdl_orig;

	cmdl_orig = GetCommandLine();
	cmdl = strchr(cmdl_orig, ' ');
	while (*cmdl == ' ') {
		cmdl++;
	}

	if (!cmdl) {
		die("Invalid command line '%s'", cmdl_orig);
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL, cmdl, NULL, NULL, FALSE, 0, NULL, NULL, &si,
			   &pi)) {
		die("CreateProcess '%s' failed (%lu)", cmdl, GetLastError());
	}

	WaitForSingleObject(pi.hProcess, INFINITE);

	if (!GetExitCodeProcess(pi.hProcess, &exitCode))
		die("GetExitCodeProcess '%s' failed (%lu)", cmdl,
		    GetLastError());

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return exitCode;
}

int file_exists(char* fn) { return _access(fn, 0) != -1; }

FILE* fopen_utf8(const char* fn, const char* mode) {
	wchar_t* wfn = NULL;
	wchar_t* wmode = NULL;
	FILE* fp = NULL;
	int wfn_len, wmode_len;

	/* Convert UTF-8 filename to wide string */
	wfn_len = MultiByteToWideChar(CP_UTF8, 0, fn, -1, NULL, 0);
	if (wfn_len == 0) {
		return NULL;
	}

	wfn = malloc(wfn_len * sizeof(wchar_t));
	if (!wfn) {
		return NULL;
	}

	if (MultiByteToWideChar(CP_UTF8, 0, fn, -1, wfn, wfn_len) == 0) {
		free(wfn);
		return NULL;
	}

	/* Convert mode to wide string */
	wmode_len = MultiByteToWideChar(CP_UTF8, 0, mode, -1, NULL, 0);
	if (wmode_len == 0) {
		free(wfn);
		return NULL;
	}

	wmode = malloc(wmode_len * sizeof(wchar_t));
	if (!wmode) {
		free(wfn);
		return NULL;
	}

	if (MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, wmode_len) == 0) {
		free(wfn);
		free(wmode);
		return NULL;
	}

	fp = _wfopen(wfn, wmode);

	free(wfn);
	free(wmode);

	return fp;
}
