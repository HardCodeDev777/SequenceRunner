#include "filedialog.hpp"

// C-like
char* unsafe_open_exe_dialog()
{
	const char* filers[] = { "*.exe" };
	// Very easy to use
	return tinyfd_openFileDialog(
		"Select executable",
		NULL,
		1,
		filers,
		NULL,
		0
	);
}

// Optional cuz it's ok if user canceled
std::optional<std::string> safe_open_exe_dialog()
{
	if (const char* res = unsafe_open_exe_dialog()) return std::string(res);
	return std::nullopt;
}