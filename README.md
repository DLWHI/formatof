# formatof

A cross-platform C++ utility for determining file formats based on magic numbers (file signatures).

## Description

formatof is a command-line and graphical tool that identifies file types by examining their binary signatures. It supports a wide range of file formats including images, videos, documents, executables, and more. The tool uses a configurable database of file signatures to provide accurate format detection.

## Features

- **Fast Detection**: Quickly identifies file formats using efficient signature matching
- **Multiple Interfaces**: Available as both command-line tool and graphical application
- **Extensive Format Support**: Includes signatures for hundreds of file types across various categories
- **Cross-Platform**: Works on Windows, Linux, and other platforms supported by CMake
- **Configurable**: File signatures can be customized via CSV files
- **Threading Support**: CLI version supports multi-threaded processing for batch operations

## Installation

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Qt6 (for GUI version, optional)

### Building

1. To build utility one may follow usual commands of cmake project building:
   ```bash
   cd formatof
   cmake -S . -B build
   cmake --build build
   cmake --install build
   ```

## Usage

### Command Line Interface

Basic usage:
```bash
formatof <file1> [file2] [file3] ...
```

Examples:
```bash
# Check a single file
formatof document.pdf

# Check multiple files
formatof image.jpg video.mp4 archive.zip

# Output example:
# document.pdf: PDF (Portable Document Format)
# image.jpg: JPEG (Joint Photographic Experts Group)
# video.mp4: MP4 (MPEG-4 Part 14)
# archive.zip: ZIP (ZIP Archive)
```

### Graphical User Interface

Launch the GUI version:
```bash
formatof-gui
```

The GUI provides a user-friendly interface for:
- Drag-and-drop file selection
- Batch processing
- Visual format identification
- Detailed format information

## Supported Formats

The tool supports detection of numerous file formats including:

- **Multimedia**: MP4, AVI, MP3, JPEG, PNG, GIF, etc.
- **Documents**: PDF, DOC, XLS, PPT
- **Archives**: ZIP, RAR, 7Z, TAR
- **Executables**: EXE, DLL, ELF, Mach-O
- **System Files**: ISO, DMG, VHD
- **And many more...**

### Adding New Signatures

To add support for new file formats:

1. Edit `resources/file_sigs.csv`
2. Format: `"Description";"Hex Signature";"Extensions";"Category";"Offset"`

Example:
```
"New Format";"DE AD BE EF";"NEW";"Miscellaneous";"0"
```

