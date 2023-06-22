DOTNET_PATH		?=dotnet

BUILDERLIB_DIR?=Tools/BuilderLib
BUILDER_DIR   ?=Tools/Builder
DEBUGPIPE_DIR ?=Tools/DebugPipe
RAMFS_DIR     ?=Tools/RAMFS

builderlib:
	$(DOTNET_PATH) build $(BUILDERLIB_DIR)/BuilderLib.csproj
builder: $(builderlib)
	$(DOTNET_PATH) build $(BUILDER_DIR)/Builder.csproj
debugpipe: $(builderlib)
	$(DOTNET_PATH) build $(DEBUGPIPE_DIR)/DebugPipe.csproj
ramfs: $(builderlib)
	$(DOTNET_PATH) build $(RAMFS_DIR)/RAMFS.csproj

macos: builder ramfs
	$(BUILDER_DIR)/bin/Debug/net7.0/Builder Kernel/macos.script
linux: builder ramfs
	$(BUILDER_DIR)/bin/Debug/net7.0/Builder Kernel/Linux.script
windows: builder debugpipe ramfs
	$(BUILDER_DIR)/bin/Debug/net7.0/Builder Kernel/builder.script
