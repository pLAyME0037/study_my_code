using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using RegistrationApp.Models;
using RegistrationApp.MyTools;

namespace RegistrationApp
{
    public partial class FmRegistrationAdd : Form
    {
        public int UserId { get; set; }
        public FmRegistrationAdd()
        {
            InitializeComponent();
        }

        private void FmRegistrationAdd_Load(object sender, EventArgs e)
        {
            CbSex.Items.Add("M");
            CbSex.Items.Add("F");
            CbSex.SelectedIndex = 0;
            CbSex.DropDownStyle = ComboBoxStyle.DropDownList;            

        }
        private void ClearForm()
        {
            TxtStudentCode.Text = "";
            TxtName.Text = "";
            CbSex.SelectedIndex = 0;
            DtpDoB.Value = DateTime.Now;
            TxtStudentCode.Focus();
        }

        private void BtClear_Click(object sender, EventArgs e)
        {
            ClearForm();
        }

        private void BtRegister_Click(object sender, EventArgs e)
        {
            RegistrationModel RegistrationMd = new RegistrationModel();

            string Msg = RegistrationMd.RegistrationAdd(TxtStudentCode.Text,TxtName.Text,CbSex.Text,Convert.ToDateTime(DtpDoB.Text),UserId);
            MessageBox.Show(Msg, "Information");
            if (Msg.Remove(12) == "Successfully")
            {
                ClearForm();
            }
        }
    }
}
