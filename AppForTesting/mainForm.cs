namespace AppForTesting
{
    using System;
    using System.Drawing;
    using System.Runtime.InteropServices;
    using System.Windows.Forms;

    /// <summary>
    /// Main form
    /// </summary>
    public partial class mainForm : Form
    {

        /// <summary>
        /// TextOut
        /// </summary>
        /// <param name="hdc">device context</param>
        /// <param name="nXStart">x</param>
        /// <param name="nYStart">y</param>
        /// <param name="lpString">string to print</param>
        /// <param name="cbString">size string</param>
        /// <returns>true if successful</returns>
        [DllImport("gdi32.dll", CharSet = CharSet.Unicode)]
        private static extern bool TextOut(IntPtr hdc, int nXStart, int nYStart, string lpString, int cbString);

        /// <summary>
        /// Constructor
        /// </summary>
        public mainForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Form painted
        /// </summary>
        /// <param name="sender">sender</param>
        /// <param name="e">event args</param>
        private void mainForm_Paint(object sender, PaintEventArgs e)
        {
            using (Graphics g = this.CreateGraphics())
            {
                IntPtr hdc = g.GetHdc();
                String s = "hello world!";
                bool b = mainForm.TextOut(hdc, 0, 0, s, s.Length);
            }

        }
    }
}
