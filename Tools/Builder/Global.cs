using System;

namespace AerOS.Builder;

public static class Global
{
    public static string Path       = "";
    public static string Compiler   = "";
    public static string Assembler  = "";
    public static string Linker     = "";
    public static string ObjectDump = "";
    public static string Grub       = "";
    public static string Limine     = "";
    public static string Emulator   = "";
    public static string RAMFSMgr   = "";
    public static string Pipe       = "";

    public static bool IsGrub       = false;
    public static bool IsLimine     = false;
    public static bool IsObjectDump = false;
}