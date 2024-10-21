from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps

class MyProjectConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    
    requires = "gtest/1.13.0" 
    def requirements(self):
        if self.settings.os == "Windows":
            self.requires("winflexbison/2.5.25")
        else:
            self.requires("flex/2.6.4")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        
        deps = CMakeDeps(self)
        deps.generate()
