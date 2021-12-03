// unfinished

// Generates 'wordlists' (not actually words) for
// bruteforce attacks (e.g. aircrack-ng, hashcat)

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#define CHARS_LENGTH 6
#define WRITE_INTERVAL 10000

using namespace std;

bool equalsIgnoreCase(const string& a, const string& b) {
	const unsigned int sz = a.size();
	if (b.size() != sz) {
		return false;
	}
	for (unsigned int i = 0; i < sz; ++i) {
		if (tolower(a[i]) != /*tolower(*/b[i])/*)*/ {
			return false;
		}
	}
	return true;
}

string humanReadableBytes1024(const long long unsigned int b) {
	const string names[9] = {"B","KiB","MiB","GiB","TiB","PiB","EiB","ZiB","YiB"};
	long long unsigned int v = b;
	int i = 0;
	while (v > 1024*1024 && i < 9) {
		v = v / 1024;
		i++;
	}
	string output = "";
	output.append(to_string(v));
	output.append(names[i]);
	return output;
}

string humanReadableBytes1000(const long long unsigned int b) {
	const string names[9] = {"B","KB","MB","GB","TB","PB","EB","ZB","YB"};
	long long unsigned int v = b;
	int i = 0;
	while (v > 1000*1000 && i < 9) {
		v = v / 1000;
		i++;
	}
	string output = "";
	output.append(to_string(v));
	output.append(names[i]);
	return output;
}

int main() {
	const string chars[CHARS_LENGTH] = {
		"0123456789",
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"!@#$%^&*()",
		"/\\,.;:'\"",
		"<>[]{}"
	};

	cout << "\n\n1: Generate passwords\n2: Combine words\n\n";
	string v;
	int n;
	while (true) {
		cout << "Enter 1 or 2 > ";
		getline(cin, v);
		try {
			n = stoi(v);
		}
		catch (...) {
			cout << "Error\n";
			continue;
		}
		if (n <= 0 || n >= 2) {
			cout << "Error\n";
			continue;
		}
		break;
	}
	if (n == 1) {
		string charsUsing = "";
		while (true) {
			for (int i=0;i<CHARS_LENGTH;i++) {
				cout << "Use " << chars[i] << " [y/n] > ";
				string yn;
				getline(cin, yn);
				if (yn == "y" || yn == "Y") {
					charsUsing.append(chars[i]);
				}
			}
			cout << "Extra characters (press Return to ignore) > ";
			string extra;
			getline(cin, extra);
			charsUsing.append(extra);
			if (charsUsing.size() == 0) {
				cout << "\nError: Select at least one character.\n\n";
				continue;
			}
			break;
		}

		string filename;
		while (true) {
			cout << "Filename > ";
			getline(cin, filename);
			if (filename.size() == 0) {
				cout << "\nError: Filename can't be empty.\n\n";
				continue;
			}
			break;
		}

		string p;
		int length;
		while (true) {
			cout << "Password length > ";
			getline(cin, p);
			try {
				length = stoi(p);
			}
			catch (...) {
				cout << "\nError: Enter a number larger than zero.\n\n";
				continue;
			}
			if (length <= 0) {
				cout << "\nError: Enter a number larger than zero.\n\n";
				continue;
			}
			break;
		}

		string d;
		string lineEnd;
		while (true) {
			cout << "Line ending (LF, CRLF or CR) > ";
			getline(cin, d);
			if (equalsIgnoreCase(d, "lf")) {
				lineEnd = "\n";
				break;
			} else if (d == "") {
				lineEnd = "\n";
				cout << "\nUsing default: LF.\n";
				break;
			} else if (equalsIgnoreCase(d, "crlf")) {
				lineEnd = "\r\n";
				break;
			} else if (equalsIgnoreCase(d, "cr")) {
				lineEnd = "\r";
				break;
			} else {
				cout << "\nError: Enter LF, CRLF or CR.\n\n";
				//continue;
			}
		}

		long long unsigned int passwords = pow(charsUsing.size(), length);
		long long unsigned int bytes =     passwords * (length + lineEnd.size());

		cout << "\nCharacters:         " << charsUsing <<
			"\nOutput file:        " << filename <<
			"\nPassword length:    " << length <<
			"\nPasswords:          " << passwords <<
			"\nExpected file size: " << humanReadableBytes1000(bytes) << ", " << humanReadableBytes1024(bytes) <<
			"\n\nStart generating? [y/n] > ";

		string yn2;
		getline(cin, yn2);
		if (yn2 != "y" && yn2 != "Y") {
			return 0;
		}

		int s[length] = {0};
		
		ofstream file;
		file.open(filename, ios::out | ios::trunc | ios::binary);
		if (!file.is_open()) {
			cout << "Error when trying to open file.";
			return 1;
		}
		string cache = "";
		int write = 0;
		while (true) {
			bool add = true;
			for (int i=0;i<length;i++) {
				cache.append({charsUsing[s[i]]});
			}
			for (int i=length-1;i>=0;i--) {
				if (add) {
					s[i]++;
					if (s[i] < charsUsing.size()) {
						add = false;
					} else {
						s[i] = 0;
					}
				}
			}
			cache.append(lineEnd);
			if (add) {
				break;
			}
			write++;
			if (write == WRITE_INTERVAL) {
				write = 0;
				file << cache;
				cache = "";
			}
			
		}
		file << cache;
		file.close();
		cout << "Finished\n";
		
	} else if (n == 2) {
		cout << "Coming soon...";
	}
	return 0;
}
