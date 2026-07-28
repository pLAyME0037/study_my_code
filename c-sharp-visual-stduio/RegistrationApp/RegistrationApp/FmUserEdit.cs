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
    public partial class FmUserEdit : Form
    {
        public int UserId { get; set; }
        public int UserIdChange { get; set; }
        public FmUserEdit()
        {
            InitializeComponent();
        }

        private void FmUserEdit_Load(object sender, EventArgs e)
        {
            DefaultFunction();
        }
        private void DefaultFunction()
        {
            UserModel UserMd = new UserModel();

            UserModel UserMdChange = new UserModel();
            UserMdChange = UserMd.UserInfor(UserIdChange);

            TxtFullname.Text = UserMdChange.Fullname;
            TxtUsername.Text = UserMdChange.Username;

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
            CbUserType.Text = UserMdChange.UserTypeName;

            //Load data in Sex ComboBox
            CbSex.Items.Clear();
            CbSex.Items.Add("M");
            CbSex.Items.Add("F");
            if (CbSex.Items.Count > 0)
            {
                CbSex.SelectedIndex = 0; // Selects the first item
            }
            CbSex.Text = UserMdChange.Sex;
        }

        private void BtChange_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(UserIdChange.ToString());

            string CbUserTypeValue = "";
            if (CbUserType.Text != "")
            {
                mycombobox itm = (mycombobox)CbUserType.SelectedItem;
                CbUserTypeValue = itm.Value.ToString();
            }

            UserModel UserMd = new UserModel();            
            string Msg = UserMd.UserChange(UserIdChange, Convert.ToInt32(CbUserTypeValue), TxtFullname.Text,CbSex.Text,UserId);
            MessageBox.Show(Msg, "Information");
            if (Msg.Remove(12) == "Successfully")
            {
                DefaultFunction();
            }
        }
    }
}
