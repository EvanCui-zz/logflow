using System;

namespace LogFlow.Viewer
{
    using System.Drawing;

    public class TagColors : ICloneable
    {
        public object Clone() => new TagColors() { Tag1 = this.Tag1, Tag2 = this.Tag2, Tag3 = this.Tag3, };

        public Color Tag1 { get; set; } = Color.Blue;
        public Color Tag2 { get; set; } = Color.Purple;
        public Color Tag3 { get; set; } = Color.Lime;
    }
}
