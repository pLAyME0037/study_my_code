using WinFormsApp1.Model;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StudentModel studentModel = new StudentModel();
            MessageBox.Show(studentModel.studentAdd(
                textBoxName.Text,
                textBoxSex.Text
            ));
            textBoxName.Text = "";
            textBoxSex.Text = "";
            textBoxName.Focus();
        }

        private void buttonDefaulltForm_Click(object sender, EventArgs e)
        {
            DefaultForm();
        }

        private void DefaultForm()
        {
            textBoxName.Text = "";
            textBoxSex.Text = "";
            textBoxName.Focus();
        }
    }
}
