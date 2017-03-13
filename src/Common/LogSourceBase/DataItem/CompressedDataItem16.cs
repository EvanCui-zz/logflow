namespace LogFlow.DataModel
{
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Explicit, Size = 16)]
    public struct CompressedDataItem16
    {
        public const int StateSize = 2;

        public CompressState State
        {
            get { return (CompressState)GetValue(this.FirstField, 0, StateSize); }
            set { SetValue(ref this.FirstField, 0, StateSize, (int)value); }
        }

        public const int LevelSize = 3;

        public int Level
        {
            get { return GetValue(this.FirstField, StateSize, LevelSize); }
            set { SetValue(ref this.FirstField, StateSize, LevelSize, value); }
        }

        public const int TimeSize = 28;

        public int TimeOffsetSeconds
        {
            get { return GetValue(this.FirstField, StateSize + LevelSize, TimeSize); }
            set { SetValue(ref this.FirstField, StateSize + LevelSize, TimeSize, value); }
        }

        // note: 32 will easily lead to a bug, that the >> operation fails.
        public const int TemplateIdSize = 31;

        public int TemplateId
        {
            get { return GetValue(this.FirstField, StateSize + LevelSize + TimeSize, TemplateIdSize); }
            set { SetValue(ref this.FirstField, StateSize + LevelSize + TimeSize, TemplateIdSize, value); }
        }

        public const int PidSize = 16;

        public int Pid
        {
            get { return GetValue(this.SecondField, 0, PidSize); }
            set { SetValue(ref this.SecondField, 0, PidSize, value); }
        }

        public const int TidSize = 18;

        public int Tid
        {
            get { return GetValue(this.SecondField, PidSize, TidSize); }
            set { SetValue(ref this.SecondField, PidSize, TidSize, value); }
        }

        public const int AidSize = 20;

        public int Aid
        {
            get { return GetValue(this.SecondField, PidSize + TidSize, AidSize); }
            set { SetValue(ref this.SecondField, PidSize + TidSize, AidSize, value); }
        }

        public const int FileIndexSize = 10;

        public int FileIndex
        {
            get { return GetValue(this.SecondField, PidSize + TidSize + AidSize, FileIndexSize); }
            set { SetValue(ref this.SecondField, PidSize + TidSize + AidSize, FileIndexSize, value); }
        }

        private static int GetValue(ulong field, int start, int length)
        {
            return (int)((field << start) >> (64 - length));
        }

        private static void SetValue(ref ulong field, int start, int length, long value)
        {
            field = (start == 0 ? 0 : ((field >> (64 - start)) << (64 - start)))
                | (ulong)value << (64 - start - length)
                | (start + length == 64 ? 0 : ((field << (start + length)) >> (start + length)));
        }

        [FieldOffset(8)]
        public int Index;

        [FieldOffset(0)]
        private ulong FirstField;
        [FieldOffset(8)]
        private ulong SecondField;
    }
}
