using System.IO.Pipes;
using System.Text;

using (var pipe = new NamedPipeClientStream(".", "TypeCPipe", PipeDirection.In))
{
    Console.WriteLine("Connecting...");
    pipe.Connect();
    Console.WriteLine("Connected");
    List<byte> data = new();
    while (pipe.IsConnected)
    {
        Console.Write(Encoding.ASCII.GetString(new byte[] { (byte)pipe.ReadByte() }));
    }
}