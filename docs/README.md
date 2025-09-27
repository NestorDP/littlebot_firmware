# LittleBot Firmware Documentation

This directory contains the generated documentation for the LittleBot firmware project.

## Generating Documentation

To generate the documentation, run:

```bash
doxygen Doxyfile
```

The documentation will be generated in the `docs/html/` directory.

## Viewing Documentation

Open `docs/html/index.html` in your web browser to view the documentation.

## Documentation Structure

- **API Reference**: Complete function and structure documentation
- **File Index**: All source and header files with descriptions
- **Module Organization**: Functions and types grouped by functionality
- **Call Graphs**: Function call relationships (if Graphviz is installed)

## Requirements

- Doxygen (install with `sudo apt install doxygen` on Ubuntu)
- Graphviz (optional, for call graphs: `sudo apt install graphviz`)