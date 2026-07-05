using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Microsoft.Reporting.WinForms;
using RegistrationApp.Models;

namespace RegistrationApp
{
    public partial class FmRegistrationListReport : Form
    {
        public FmRegistrationListReport()
        {
            InitializeComponent();
        }

        private void FmRegitrationListReport_Load(object sender, EventArgs e)
        {
            RegistrationModel RegistrationMd = new RegistrationModel();
            
            //reportViewer1.LocalReport.DataSources.Add(t);
            reportViewer1.LocalReport.DataSources.Clear();
            ReportDataSource Rpdts = new ReportDataSource("DataSet1", RegistrationMd.RegistrationList_DTT());
            reportViewer1.LocalReport.DataSources.Add(Rpdts);

            this.reportViewer1.RefreshReport();
        }
    }
}
