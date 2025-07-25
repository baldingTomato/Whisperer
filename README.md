# Whisperer

**Whisperer** is a **fast, minimalist translation assistant** that helps you stay focused by translating selected text without jumping through windows.

### ✅ What it does:

- Listens for a global keyboard shortcut (e.g., `Ctrl + L`)
- Reads the currently selected text
- Translates the text from English to Polish via the [Lingva Translate API](https://github.com/TheDavidDelta/lingva-translate)
- Displays the translation in a temporary popup overlay
- All of this happens without stealing window focus or interrupting your workflow

---

### 🧩 Architecture Overview

| Component         | Responsibility                                                                 |
|------------------|---------------------------------------------------------------------------------|
| `main.cpp`        | Program entry point; instantiates and runs `Whisperer`                         |
| `Whisperer`       | Main orchestrator: wires together the listener, translator, and popup          |
| `Listener`        | Interface for detecting keyboard shortcuts (platform-specific implementations) |
| `Translator`      | Translates input using an abstract API                                         |
| `TranslationAPI`  | Interface for translation services; `LingvaAPI` is the current implementation  |
| `Popup`           | Interface for displaying translation overlays (OS-specific implementations)    |
| `ClipboardReader` | Abstracts access to selected or clipboard text                                 |
| `InputDevice`     | Linux-specific interface to listen for raw input events                        |

All platform-specific logic is abstracted behind interfaces (e.g., `LinuxListener`, `WindowsPopup`) to ensure maintainable cross-platform support.

---

| Platform | Supported | Notes                                 |
| -------- | --------- | ------------------------------------- |
| Linux    | ✅         | Tested on Hyprland (Wayland)          |
| Windows  | ✅         | Builds and runs with Qt + VCPKG setup |
| macOS    | 🚧        | Planned (not yet implemented)         |

---

### 🛠️ Building the Project

```bash
# Create a build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project (Release configuration recommended)
cmake --build . --config Release
```

---

Feel free to contribute, open issues, or suggest improvements! I would appreciate any insights on bugs or improvements.
