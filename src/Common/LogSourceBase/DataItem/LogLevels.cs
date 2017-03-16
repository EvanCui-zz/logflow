namespace LogFlow.DataModel
{
    using System;

    [Flags]
    public enum LogLevels
    {
        None = 0x0,
        Critical = 0x1,
        Error = 0x2,
        Warning = 0x4,
        Detail = 0x8,
        Info = 0x10,
        Verbose = 0x20,
    }
}
