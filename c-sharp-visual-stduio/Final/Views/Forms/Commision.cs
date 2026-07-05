using Final.Helpers;
using Final.Models;
using System;
using System.Globalization;
using System.Windows.Forms;

namespace Final.Views.Forms
{
    public partial class Commision : Form
    {
        public Commision()
        {
            InitializeComponent();
        }

        private void btChkCom_Click(object sender, EventArgs e)
        {
            if (!decimal.TryParse(
                    this.txbSale.Text, 
                    NumberStyles.Number, 
                    CultureInfo.CurrentCulture, 
                    out decimal saleAmount
            )) {
                MessageBox.Show(
                    this, 
                    "Invalid sale amount. Please enter a numeric value.", 
                    "Input Error", 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Warning
                );
                this.txbSale.Focus();
                this.txbSale.SelectAll();
                return;
            }

            SaleCommission commission = new SaleCommission();
            decimal cal = commission.CalCommision(saleAmount);

            this.txbCommision.Text = cal.ToString("N2", CultureInfo.CurrentCulture);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            NavigationManager.NavigateBack(this);
        }
    }
}
