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
    public partial class FmUserAdd : Form
    {
        public int UserId { get; set; }
        public FmUserAdd()
        {
            InitializeComponent();
        }

        private void FmUserAdd_Load(object sender, EventArgs e)
        {
            DefaultFunction();
        }

        private void DefaultFunction()
        {
            TxtUsername.Text = "";
            TxtFullname.Text = "";
            TxtPassword.Text = "";
            TxtPassworAgain.Text = "";

            //Load data in User Type ComboBox
            UserTypeModel UserTyeMd = new UserTypeModel();
            CbUserType.Items.Clear();
            foreach (var Md in UserTyeMd.UserTypeList())
            {
                CbUserType.Items.Add(new mycombobox(Md.UserTypeName, Md.UserTypeId));

            }
            if (CbUserType.Items.Count > 0)
            {
                CbUserType.SelectedIndex = 0; // Selects the first item
            }

            //Load data in Sex ComboBox
            CbSex.Items.Clear();
            CbSex.Items.Add("M");
            CbSex.Items.Add("F");
            if (CbSex.Items.Count > 0)
            {
                CbSex.SelectedIndex = 0; // Selects the first item
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DefaultFunction();
        }

        private void BtAdd_Click(object sender, EventArgs e)
        {
            string CbUserTypeValue = "";
            if (CbUserType.Text != "")
            {
                mycombobox itm = (mycombobox)CbUserType.SelectedItem;
                CbUserTypeValue = itm.Value.ToString();
            }

            UserModel UserMd = new UserModel();
            string Msg = UserMd.UserAdd(Convert.ToInt32(CbUserTypeValue), TxtUsername.Text,TxtFullname.Text,CbSex.Text,TxtPassword.Text,TxtPassworAgain.Text,UserId);
            MessageBox.Show(Msg, "Information");
            if(Msg.Remove(12) == "Successfully")
            {
                DefaultFunction();
            }
        }
    }
}
