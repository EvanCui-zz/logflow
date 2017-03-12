using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    [StructLayout(LayoutKind.Explicit, Size = 8)]
    public struct CompressedDataItem8
    {
        public bool IsCompressed
        {
            get { return (this.pid & 0x1) == 1; }
            set { this.pid = (byte)(value ? (this.pid | 1) : (this.pid & ~1)); }
        }

        public byte Pid
        {
            get { return (byte)(this.pid >> 1); }
            set { this.pid = (byte)((value << 1) | (this.pid & 1)); }
        }

        public byte FileIndex
        {
            get { return (byte)(this.fileIndexLevel >> 3); }
            set { this.fileIndexLevel = (byte)((value << 3) | (this.fileIndexLevel & 7)); }
        }

        public byte Level
        {
            get { return (byte)(this.fileIndexLevel & 7); }
            set { this.fileIndexLevel = (byte)(value | (this.fileIndexLevel & ~7)); }
        }

        [FieldOffset(4)]
        public int Index;

        [FieldOffset(0)]
        private byte pid;
        [FieldOffset(1)]
        public byte Tid;
        [FieldOffset(2)]
        public byte Aid;
        [FieldOffset(3)]
        private byte fileIndexLevel;

        [FieldOffset(4)]
        public ushort TimeOffsetSeconds;
        [FieldOffset(6)]
        public ushort TemplateId;
    }
}
