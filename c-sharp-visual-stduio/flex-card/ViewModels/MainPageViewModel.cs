using user_info.Models;
using System.Collections.ObjectModel;

namespace user_info.ViewModels;
public class MainPageViewModel
{
    public ObservableCollection<Product> Products { get; }

    public MainPageViewModel() {
        Products = new ObservableCollection<Product> {
            new Product {
                Name  = "Laptop",
                Price = "$1200",
                Image = "Laptop.jpg"
            },
            new Product {
                Name  = "Keyboard",
                Price = "$12",
                Image = "Keyboard.jpg"
            },
            new Product {
                Name  = "Mouse",
                Price = "$8",
                Image = "Mouse.jpg"
            },
            new Product {
                Name  = "Monitor",
                Price = "$300",
                Image = "Monitor.jpg"
            },
            new Product {
                Name  = "I Phone",
                Price = "$800",
                Image = "Iphone.jpg"
            },
            new Product {
                Name  = "Laptop",
                Price = "$800",
                Image = "Laptop.jpg"
            },
            new Product {
                Name  = "Headphone",
                Price = "$80",
                Image = "Headphone.jpg"
            },
        };
    }
}
