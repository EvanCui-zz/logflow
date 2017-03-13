namespace LogFlow.DataModel
{
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Explicit, Size = 8)]
    public struct CompressedDataItem8
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

        public const int TidSize = 11;
        public int Tid
        {
            get { return GetValue(this.FirstField, StateSize + LevelSize, TidSize); }
            set { SetValue(ref this.FirstField, StateSize + LevelSize, TidSize, value); }
        }

        public const int TimeSize = 16;
        public int TimeOffsetSeconds
        {
            get { return GetValue(this.FirstField, StateSize + LevelSize + TidSize, TimeSize); }
            set { SetValue(ref this.FirstField, StateSize + LevelSize + TidSize, TimeSize, value); }
        }

        public const int PidSize = 5;
        public int Pid
        {
            get { return GetValue(this.SecondField, 0, PidSize); }
            set { SetValue(ref this.SecondField, 0, PidSize, value); }
        }

        public const int AidSize = 11;
        public int Aid
        {
            get { return GetValue(this.SecondField, PidSize, AidSize); }
            set { SetValue(ref this.SecondField, PidSize, AidSize, value); }
        }

        public const int TemplateIdSize = 10;
        public int TemplateId
        {
            get { return GetValue(this.SecondField, PidSize + AidSize, TemplateIdSize); }
            set { SetValue(ref this.SecondField, PidSize + AidSize, TemplateIdSize, value); }
        }

        public const int FileIndexSize = 6;
        public int FileIndex
        {
            get { return GetValue(this.SecondField, PidSize + AidSize + TemplateIdSize, FileIndexSize); }
            set { SetValue(ref this.SecondField, PidSize + AidSize + TemplateIdSize, FileIndexSize, value); }
        }

        private static int GetValue(uint field, int start, int length)
        {
            return (int)((field << start) >> (32 - length));
        }

        private static void SetValue(ref uint field, int start, int length, int value)
        {
            field = (start == 0 ? 0 : ((field >> (32 - start)) << (32 - start)))
                | (uint)value << (32 - start - length)
                | (start + length == 32 ? 0 : ((field << (start + length)) >> (start + length)));
        }

        [FieldOffset(4)]
        public int Index;

        // pid 4 bit, (0-7) tid 11 bit, (0-2047) compress 1 bit (0-1)
        [FieldOffset(0)]
        private uint FirstField;
        [FieldOffset(4)]
        private uint SecondField;
    }
}
