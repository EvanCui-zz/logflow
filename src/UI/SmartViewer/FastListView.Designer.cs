using System.Windows.Forms;

namespace LogFlow.Viewer
{
    partial class FastListView : ListView
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
                foreach(var b in this.LevelBrushes) { b.Dispose(); }
                this.LevelBrushes.Clear();
                this.BoldFont.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // FastListView
            // 
            this.Name = "FastListView";
            this.Size = new System.Drawing.Size(393, 359);           
            this.ResumeLayout(false);
        }

        #endregion
    }
}
