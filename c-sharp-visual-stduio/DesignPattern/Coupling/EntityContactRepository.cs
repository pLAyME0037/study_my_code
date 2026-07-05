using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Coupling
{
    // 3. Concrete Repository (Implementation - e.g., Entity Framework or Mock)
    public class EntityContactRepository : IContactRepository
    {
        private List<Contact> _contacts = new List<Contact>();
        private int _nextId = 1;

        public EntityContactRepository()
        {
            // Seed data
            Add(new Contact { Name = "Alice", Email = "alice@example.com" });
            Add(new Contact { Name = "Bob", Email = "bob@example.com" });
        }

        public IEnumerable<Contact> GetAll() => _contacts;

        public Contact GetById(int id) => _contacts.FirstOrDefault(c => c.Id == id);

        public void Add(Contact contact)
        {
            contact.Id = _nextId++;
            _contacts.Add(contact);
        }

        public void Update(Contact contact)
        {
            var index = _contacts.FindIndex(c => c.Id == contact.Id);
            if (index != -1)
                _contacts[index] = contact;
        }

        public void Delete(int id)
        {
            var contact = GetById(id);
            if (contact != null)
                _contacts.Remove(contact);
        }
    }
}
