using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Coupling
{
    public class ContactController
    {
        private readonly IContactService _service;

        // Inject service (further abstraction)
        public ContactController(IContactService service)
        {
            _service = service;
        }

        public void ListAll()
        {
            var contacts = _service.GetAllContacts();
            foreach (var c in contacts)
            {
                Console.WriteLine($"ID: {c.Id}, Name: {c.Name}, Email: {c.Email}");
            }
        }

        public void AddNewContact(string name, string email)
        {
            var contact = new Contact { Name = name, Email = email };
            if (_service.CreateContact(contact))
                Console.WriteLine("Contact added successfully.");
            else
                Console.WriteLine("Failed to add contact. Check input.");
        }
    }
}
