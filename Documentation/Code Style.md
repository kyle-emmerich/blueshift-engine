#Code Style
Blueshift requires a high-quality codebase that does not make compromises. The *entire* engine must be exception-safe, const-correct, and free of memory leaks. Usage of raw pointers should be documented and ownership should always be managed by the engine.

None of these rules should be arbitrary or go without justification. While Titan Interactive is still small, it's worth planning for future growth. Documenting a good code style is a major part of that, as all software engineers will eventually need to memorize and use it.

##Modules/Namespaces
Blueshift's code uses the concept of modules. Each module occupies a folder within the source and contains class definitions. The class definitions shall reside within a namespace named for the module. The folder structure should match the namespace structure.

Each module should have a header named after the module, and it will be responsible for including everything in the module in the proper order to account for circular dependencies/forward declarations.

##Header Files
Headers should have the file extension `.h` and should contain one major class and optionally several subordinate classes. The file will be named after the major class.

All headers should be self-contained and include what they need to compile correctly when possible. However, the module system described above should be able to resolve any other issues.

