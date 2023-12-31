﻿using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using AerOS.Builder.Library;

namespace AerOS.Builder;

public static class Program
{    
    private static void Main(string[] args)
    {
        Debug.Log("AerOS Builder Utility\n");
        RegisterCommands();
        
        if (args.Length == 0) { Debug.Error("No input file specified."); }
        else
        {
            if (!File.Exists(args[0])) { Debug.Error("Unable to locate command file at '%s'", args[0]); }
            string[] lines = File.ReadAllLines(args[0]);
            foreach (string line in lines) { if (line.StartsWith(";")) { continue; } CommandParser.Execute(line); }
        }
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows)) {
            Console.Read(); // why? -nick // lets just enable it for our special snowflake -kev
        }
    }

    private static void RegisterCommands()
    {
        CommandParser.Register(CommandDeclarations.INFO);
        CommandParser.Register(CommandDeclarations.SET_ASSEMBLER);
        CommandParser.Register(CommandDeclarations.SET_COMPILER);
        CommandParser.Register(CommandDeclarations.SET_LINKER);
        CommandParser.Register(CommandDeclarations.SET_OBJDUMP);
        CommandParser.Register(CommandDeclarations.SET_GRUB);
        CommandParser.Register(CommandDeclarations.SET_LIMINE);
        CommandParser.Register(CommandDeclarations.SET_EMULATOR);
        CommandParser.Register(CommandDeclarations.SET_RAMFSMGR);
        CommandParser.Register(CommandDeclarations.SET_PIPE);
        CommandParser.Register(CommandDeclarations.SET_DIR);
        CommandParser.Register(CommandDeclarations.MK_DIR);
        CommandParser.Register(CommandDeclarations.RM_DIR);
        CommandParser.Register(CommandDeclarations.RMMK_DIR);
        CommandParser.Register(CommandDeclarations.COMPILE);
        CommandParser.Register(CommandDeclarations.COMPILE_PATH);
        CommandParser.Register(CommandDeclarations.ASSEMBLE);
        CommandParser.Register(CommandDeclarations.ASSEMBLE_PATH);
        CommandParser.Register(CommandDeclarations.LINK);
        CommandParser.Register(CommandDeclarations.LINK_PATH);
        CommandParser.Register(CommandDeclarations.MK_ISO);
        CommandParser.Register(CommandDeclarations.MK_RAMDSIK);
        CommandParser.Register(CommandDeclarations.LIMINE);
        CommandParser.Register(CommandDeclarations.RUN);
        CommandParser.Register(CommandDeclarations.PIPE);
    }
}