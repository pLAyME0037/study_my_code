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
namespace RegistrationApp
{
    public partial class FmUserManagement : Form
    {
        public int UserId { get; set; }
        public FmUserManagement()
        {
            InitializeComponent();

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "UserId", DataPropertyName = "UserId" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "UserTypeName", DataPropertyName = "UserTypeName" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "Username", DataPropertyName = "Username" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "Fullname", DataPropertyName = "Fullname" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "Sex", DataPropertyName = "Sex" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "Status", DataPropertyName = "Status" });

            dataGridView1.Columns[0].Width = 80;
            dataGridView1.Columns[1].Width = 150;
            dataGridView1.Columns[2].Width = 100;
            dataGridView1.Columns[3].Width = 220;
            dataGridView1.Columns[4].Width = 80;
            dataGridView1.Columns[5].Width = 80;

            this.dataGridView1.Columns[4].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            this.dataGridView1.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

        }

        private void FmUserManagement_Load(object sender, EventArgs e)
        {
            LoadData();
        }

        private void BtAdd_Click(object sender, EventArgs e)
        {
            FmUserAdd Fm = new FmUserAdd();
            Fm.UserId = UserId;
            Fm.ShowDialog();
            //after this form
            LoadData();

        }
        private void LoadData()
        {
            UserModel UserMd = new UserModel();
            //Binding
            //dataGridView1.DataSource = UserMd.UserList();

            //Coding
            //Clear Data in dataGridView1
            dataGridView1.Rows.Clear();

            foreach (var User in UserMd.UserList())
            {
                dataGridView1.Rows.Add(User.UserId, User.UserTypeName, User.Username, User.Fullname, User.Sex, User.EnableStatus);
            }
        }

        private void BtEdit_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {
                string FirstColData = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();

                FmUserEdit Fm = new FmUserEdit();
                Fm.UserId = UserId;
                Fm.UserIdChange = Convert.ToInt32(FirstColData);
                Fm.ShowDialog();
                //after this form
                LoadData();
            }
               
        }

        private void BtDelete_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {                
                DialogResult result = MessageBox.Show("Are sure to delete this user?", "Delete Confirmation", MessageBoxButtons.YesNo);
                if (result == DialogResult.Yes)
                {
                    // User clicked Yes
                    // Perform save operation
                    string FirstColData = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();
                    UserModel UserMd = new UserModel();
                    string Msg = UserMd.UserDelete(Convert.ToInt32(FirstColData), UserId);
                    if (Msg == "")
                    {
                        LoadData();
                    }
                    else
                    {
                        MessageBox.Show(Msg,"Information");
                    }                    
                }
            }
        }

        private void dataGridView1_MouseClick(object sender, MouseEventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {
                if (e.Button == System.Windows.Forms.MouseButtons.Right)
                {
                    CmsOnDgv.Show(new System.Drawing.Point(MousePosition.X, MousePosition.Y));
                }
            }
        }

        private void tsmiRefresh_Click(object sender, EventArgs e)
        {
        }

        private void tsmiChangeStatus_Click(object sender, EventArgs e)
        {
            
        }

        private void changeStatusToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {
                DialogResult result = MessageBox.Show("Are sure to change this user status?", "Change Confirmation", MessageBoxButtons.YesNo);
                if (result == DialogResult.Yes)
                {
                    // User clicked Yes
                    // Perform save operation
                    string FirstColData = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();
                    UserModel UserMd = new UserModel();
                    string Msg = UserMd.UserChangeStatus(Convert.ToInt32(FirstColData), UserId);
                    if (Msg == "")
                    {
                        LoadData();
                    }
                    else
                    {
                        MessageBox.Show(Msg, "Information");
                    }
                }
            }
        }

        private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadData();
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {
            BtEdit.PerformClick();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            BtDelete.PerformClick();
        }

        private void FmUserManagement_KeyDown(object sender, KeyEventArgs e)
        {
            
        }

        private void changePasswordToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {
                string FirstColData = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();
                FmUserChangePassword Fm = new FmUserChangePassword();
                Fm.Id = Convert.ToInt32(FirstColData);
                Fm.UserId = UserId;

                Fm.ShowDialog();

            }
        }
    }
}
