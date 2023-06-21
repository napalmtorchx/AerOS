using System;
using System.IO;
using System.Text;

public static class Program
{
    public static RAMFileSystem RAMFS = new RAMFileSystem();

    private static void Main(string[] args)
    {
        Debug.Info("AerOS RAMFS Utility");

        RAMFS = new RAMFileSystem(1024, 33554432);
        SaveImage("../../Images/ramdisk.img");   
    }

    public static void SaveImage(string fname)
    {
        File.WriteAllBytes(fname, RAMFS.Data);
        Debug.Info("Saved image file to '" + fname + "'");
    }

    public static void ImportFile(string src, string dest, bool hidden = false)
    {
        if (!File.Exists(src)) { Debug.Error("Unable to locate file '" + src + "'"); return; }
        byte[] data = File.ReadAllBytes(src);
        RAMFS.AddFile(dest, data, hidden);
    }
}
