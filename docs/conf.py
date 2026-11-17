from pathlib import Path

# Documentation directory
DOCS_DIR = Path(__file__).resolve().parent

# Project information
project = "Littlebot Firmware"
author  = "Littlebot Project"
release = "0.1"

# Sphinx extensions
extensions = [
    "breathe",
]

# Breathe configuration
breathe_projects = {
    "littlebot_firmware": str(DOCS_DIR / "xml"),
}

breathe_default_project = "littlebot_firmware"

# Sphinx configuration
exclude_patterns = [
    "_build",
    "xml",
]

html_theme = "sphinx_rtd_theme"