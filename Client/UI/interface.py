import ctypes

# Load the shared library
mycppfunction = ctypes.cdll.LoadLibrary('./mycppfunction.so')