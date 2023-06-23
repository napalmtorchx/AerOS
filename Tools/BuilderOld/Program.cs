using System;
using System.Linq;
using System.Collections.Generic;

public static class Program
{
    private static void Main(string[] args)
    {
        Debug.Info("NapalmOS Build Utility");
        Debug.Info("Current path: " + Builder.Path);
        Builder.BuildAll();

        Console.ReadLine();
        Environment.Exit(0);
    }
}
