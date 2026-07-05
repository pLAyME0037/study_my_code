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
using Microsoft.Office.Interop.Excel;

namespace RegistrationApp
{
    public partial class FmRegistrationMng : Form
    {
        public int UserId { get; set; }
        public FmRegistrationMng()
        {
            InitializeComponent();

            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "RegistrationId", DataPropertyName = "RegistrationId" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "StudentCode", DataPropertyName = "StudentCode" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "Name", DataPropertyName = "Name" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "Sex", DataPropertyName = "Sex" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "DoB", DataPropertyName = "DoB" });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn { HeaderText = "C_Date", DataPropertyName = "C_Date" });

            dataGridView1.Columns[0].Width = 80;
            dataGridView1.Columns[1].Width = 150;
            dataGridView1.Columns[2].Width = 250;
            dataGridView1.Columns[3].Width = 50;
            dataGridView1.Columns[4].Width = 150;
            dataGridView1.Columns[5].Width = 100;
        }

        private void FmRegistrationMng_Load(object sender, EventArgs e)
        {
            LoadData();
        }

        private void BtAdd_Click(object sender, EventArgs e)
        {
            FmRegistrationAdd Fm = new FmRegistrationAdd();
            Fm.UserId = UserId;
            Fm.ShowDialog();

            //after this form
            LoadData();

        }

        private void BtDelete_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {
                DialogResult result = MessageBox.Show("Are sure to delete this registration?", "Delete Confirmation", MessageBoxButtons.YesNo);
                if (result == DialogResult.Yes)
                {
                    // User clicked Yes
                    // Perform save operation
                    string FirstColData = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();
                    RegistrationModel Md = new RegistrationModel();
                    string Msg = Md.RegistrationDelete(Convert.ToInt32(FirstColData), UserId);
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
        private void LoadData()
        {
            RegistrationModel RegistrationMd = new RegistrationModel();
            dataGridView1.Rows.Clear();

            foreach (var Reg in RegistrationMd.RegistrationList())
            {
                dataGridView1.Rows.Add(Reg.RegistrationId, Reg.StudentCode, Reg.Name, Reg.Sex, Reg.DoB.ToString("mm/dd/yyyy"), Reg.C_Date.ToString("mm/dd/yyyy"));
            }
        }

        private void BtReport_Click(object sender, EventArgs e)
        {
            if (this.dataGridView1.SelectedRows.Count > 0)
            {
                string FirstColData = dataGridView1.SelectedRows[0].Cells[0].Value.ToString();

                FmRegistrationReport FmRegReport = new FmRegistrationReport();
                FmRegReport.RegistrationId = Convert.ToInt32(FirstColData);

                FmRegReport.ShowDialog();
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            BtDelete.PerformClick();
        }

        private void reportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            BtReport.PerformClick();
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

        private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadData();
        }

        private void BtRegistrationList_Click(object sender, EventArgs e)
        {
            FmRegistrationListReport FmReport = new FmRegistrationListReport();
            FmReport.ShowDialog();
        }

        private void BtExcel_Click(object sender, EventArgs e)
        {
            try
            {
                Microsoft.Office.Interop.Excel.Application xla = new Microsoft.Office.Interop.Excel.Application();

                Workbook wb = xla.Workbooks.Add(XlSheetType.xlWorksheet);
                Worksheet ws = (Worksheet)xla.ActiveSheet;

                //Sets page setup properties
                ws.PageSetup.PaperSize = Microsoft.Office.Interop.Excel.XlPaperSize.xlPaperA4;
                ws.PageSetup.Orientation = Microsoft.Office.Interop.Excel.XlPageOrientation.xlPortrait;
                ws.PageSetup.CenterHorizontally = true;
                //ws.PageSetup.CenterVertically = true;

                // Normal Margins
                ws.PageSetup.LeftMargin = xla.InchesToPoints(0.3);
                ws.PageSetup.RightMargin = xla.InchesToPoints(0.3);
                ws.PageSetup.TopMargin = xla.InchesToPoints(0.6);
                ws.PageSetup.BottomMargin = xla.InchesToPoints(0.6);
                ws.PageSetup.HeaderMargin = xla.InchesToPoints(0.2);
                ws.PageSetup.FooterMargin = xla.InchesToPoints(0.2);

                //set column width
                ws.Range["A1"].ColumnWidth = 4;//or ws.Cells[1, 1]; // Cell A1
                ws.Range["B1"].ColumnWidth = 18;//or ws.Cells[1, 2]
                ws.Range["C1"].ColumnWidth = 15;//or ws.Cells[1, 3]
                ws.Range["D1"].ColumnWidth = 20;//or ws.Cells[1, 4]
                ws.Range["E1"].ColumnWidth = 10;//or ws.Cells[1, 5]
                ws.Range["F1"].ColumnWidth = 15;//or ws.Cells[1, 6]

                xla.Visible = true;
                //Title
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]].Merge();// ws.Range["A1:F1"].Merge();
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]].Font.Bold = true;
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]].VerticalAlignment = 2;
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]].Font.Size = 32;
                ws.Range[ws.Cells[1, 1], ws.Cells[1, 6]] = "Registration List";

                int ReportAt = 3;

                //Table Attributes
                int RowNumber = ReportAt - 1;
                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, RowNumber]].RowHeight = 32;

                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, 1]].Font.Bold = true;
                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, 1]].VerticalAlignment = 2;
                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, 1]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, 1]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, 1]].Font.Size = 12;
                ws.Range[ws.Cells[RowNumber, 1], ws.Cells[RowNumber, 1]] = "No";//Cell(2&1)= A2

                ws.Range[ws.Cells[RowNumber, 2], ws.Cells[RowNumber, 2]].Font.Bold = true;
                ws.Range[ws.Cells[RowNumber, 2], ws.Cells[RowNumber, 2]].VerticalAlignment = 2;
                ws.Range[ws.Cells[RowNumber, 2], ws.Cells[RowNumber, 2]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[RowNumber, 2], ws.Cells[RowNumber, 2]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[RowNumber, 2], ws.Cells[RowNumber, 2]].Font.Size = 12;
                ws.Range[ws.Cells[RowNumber, 2], ws.Cells[RowNumber, 2]] = "Registration ID"; //2&2= B2

                ws.Range[ws.Cells[RowNumber, 3], ws.Cells[RowNumber, 3]].Font.Bold = true;
                ws.Range[ws.Cells[RowNumber, 3], ws.Cells[RowNumber, 3]].VerticalAlignment = 2;
                ws.Range[ws.Cells[RowNumber, 3], ws.Cells[RowNumber, 3]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[RowNumber, 3], ws.Cells[RowNumber, 3]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[RowNumber, 3], ws.Cells[RowNumber, 3]].Font.Size = 12;
                ws.Range[ws.Cells[RowNumber, 3], ws.Cells[RowNumber, 3]] = "Student Code"; //Cell(2&3)=C2

                ws.Range[ws.Cells[RowNumber, 4], ws.Cells[RowNumber, 4]].Font.Bold = true;
                ws.Range[ws.Cells[RowNumber, 4], ws.Cells[RowNumber, 4]].VerticalAlignment = 2;
                ws.Range[ws.Cells[RowNumber, 4], ws.Cells[RowNumber, 4]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[RowNumber, 4], ws.Cells[RowNumber, 4]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[RowNumber, 4], ws.Cells[RowNumber, 4]].Font.Size = 12;
                ws.Range[ws.Cells[RowNumber, 4], ws.Cells[RowNumber, 4]] = "Name";//Cell(2&4)=D2

                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]].Font.Bold = true;
                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]].WrapText = true;
                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]].VerticalAlignment = 2;
                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]].Font.Size = 12;
                ws.Range[ws.Cells[RowNumber, 5], ws.Cells[RowNumber, 5]] = "Sex";//cell(2&5)=E2

                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]].Font.Bold = true;
                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]].WrapText = true;
                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]].VerticalAlignment = 2;
                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]].HorizontalAlignment = 3;
                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]].Font.Name = "Times New Roman";
                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]].Font.Size = 12;
                ws.Range[ws.Cells[RowNumber, 6], ws.Cells[RowNumber, 6]] = "DoB";//cell(2&6)=F2

                int RecordNumber = 0;
                //Content
                RegistrationModel RegistrationMd = new RegistrationModel();
                // Iterate through the list using foreach
                foreach (RegistrationModel Registration in RegistrationMd.RegistrationList())
                {

                    ws.Cells[ReportAt + RecordNumber, 1] = (RecordNumber + 1);//No

                    ws.Cells[ReportAt + RecordNumber, 2] = Registration.RegistrationId;//RegistrationId
                    ws.Cells[ReportAt + RecordNumber, 2].HorizontalAlignment = 3;//Center

                    ws.Cells[ReportAt + RecordNumber, 3] = Registration.StudentCode;//RegistrationId
                    ws.Cells[ReportAt + RecordNumber, 3].NumberFormat = "00000";
                    ws.Cells[ReportAt + RecordNumber, 3].HorizontalAlignment = 3;//Center

                    ws.Cells[ReportAt + RecordNumber, 4] = Registration.Name;//Name
                    ws.Cells[ReportAt + RecordNumber, 4].HorizontalAlignment = 1;//Left

                    ws.Cells[ReportAt + RecordNumber, 5] = Registration.Sex;//Sex
                    ws.Cells[ReportAt + RecordNumber, 5].HorizontalAlignment = 3;//Center

                    ws.Cells[ReportAt + RecordNumber, 6] = Registration.DoB;//DoB
                    ws.Cells[ReportAt + RecordNumber, 6].HorizontalAlignment = 3;//Center
                    RecordNumber++;
                }
                //Set Border
                ws.Range[ws.Cells[ReportAt - 1, 1], ws.Cells[RecordNumber + ReportAt - 1, 6]].Borders.LineStyle = 1;
                //Set Border
                ws.PageSetup.Zoom = false;

                ws.PageSetup.FitToPagesWide = 1;
            }
            catch (Exception)
            {
                MessageBox.Show("Ms-Excel Error, pls contact the admin user.");
            }
            
        }
    }
}
