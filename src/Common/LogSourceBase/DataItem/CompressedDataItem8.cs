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
            get { return (this.pidTidIsCompress & 0x1) == 1; }
            set { this.pidTidIsCompress = (ushort)(value ? (this.pidTidIsCompress | 1) : (this.pidTidIsCompress & ~1)); }
        }

        public int Pid
        {
            get { return (this.pidTidIsCompress >> 12); }
            set { this.pidTidIsCompress = (ushort)((value << 12) | (this.pidTidIsCompress & 0xFFF)); }
        }

        public int Tid
        {
            get { return ((this.pidTidIsCompress << 4) >> 5); }
            set { this.pidTidIsCompress = (ushort)((value << 1) | (this.pidTidIsCompress & 1) | (this.pidTidIsCompress & 0xF000)); }
        }

        public int FileIndex
        {
            get { return (this.fileIndexLevel >> 3); }
            set { this.fileIndexLevel = (byte)((value << 3) | (this.fileIndexLevel & 7)); }
        }

        public int Level
        {
            get { return (this.fileIndexLevel & 7); }
            set { this.fileIndexLevel = (byte)(value | (this.fileIndexLevel & ~7)); }
        }

        [FieldOffset(4)]
        public int Index;

        // pid 4 bit, (0-7) tid 11 bit, (0-2047) compress 1 bit (0-1)
        [FieldOffset(0)]
        private ushort pidTidIsCompress;
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
