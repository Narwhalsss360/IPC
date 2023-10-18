using System;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace PracticalC
{

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    struct PracticalObject
    {
        public uint Identifier;
        public uint State;
        public uint Value;
        public uint Host;

        public PracticalObject(uint Identifier, uint State, uint Value)
        {
            this.Identifier = Identifier;
            this.State = State;
            this.Value = Value;
            this.Host = 0;
        }
    }


    internal class Program
    {
        const string ObjectName = "PRACTICALOBJECTIDENTIFIER";
        const string FileName = "PRACTICALOBJECT.tmp";

        static PracticalObject parse(byte[] Bytes)
        {
            if (Bytes.Length != 24) return new PracticalObject(0, 0, 0);
            uint Identifier = BitConverter.ToUInt32(Bytes, 0);
            uint State = BitConverter.ToUInt32(Bytes, 4);
            uint Value = BitConverter.ToUInt32(Bytes, 8);
            return new PracticalObject(Identifier, State, Value);
        }

        static PracticalObject DeserializePO(byte[] Bytes)
        {
            IntPtr Pointer = Marshal.AllocHGlobal(24);
            Marshal.Copy(Bytes, 0, Pointer, 24);
            PracticalObject obj = (PracticalObject)Marshal.PtrToStructure(Pointer, typeof(PracticalObject));
            Marshal.FreeHGlobal(Pointer);
            return obj;
        }

        static void Main(string[] args)
        {
            string FullPath = $"{Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData)}\\Temp\\{FileName}";

            recheck:
            if (!File.Exists(FullPath))
            {
                Console.WriteLine("Object file not found! Press enter to recheck.");
                Console.ReadLine();
                goto recheck;
            }

            PracticalObject obj = new PracticalObject(0, 0, 0);
            using (var Map = MemoryMappedFile.OpenExisting(ObjectName))
            {
                using (var Accessor = Map.CreateViewAccessor(0, 24, MemoryMappedFileAccess.Read))
                {
                    var Formatter = new BinaryFormatter();
                    while (obj.State == 0)
                    {
                        byte[] Bytes = new byte[24];
                        Accessor.ReadArray<byte>(0, Bytes, 0, 24);
                        obj = DeserializePO(Bytes);
                    }
                }
            }
            Console.WriteLine($"Exited with value { obj.Value }");
        }
    }
}