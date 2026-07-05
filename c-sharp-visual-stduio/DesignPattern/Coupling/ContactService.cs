using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Coupling
{
    // 5. Service Implementation (Business Logic Layer)
    public class ContactService : IContactService
    {
        private readonly IContactRepository _repository;

        // Constructor Injection - Key to loose coupling
        public ContactService(IContactRepository repository)
        {
            _repository = repository;
        }

        public IEnumerable<Contact> GetAllContacts() => _repository.GetAll();

        public Contact GetContact(int id) => _repository.GetById(id);

        public bool CreateContact(Contact contact)
        {
            if (string.IsNullOrWhiteSpace(contact.Name) || string.IsNullOrWhiteSpace(contact.Email))
                return false;

            _repository.Add(contact);
            return true;
        }

        public bool UpdateContact(Contact contact)
        {
            if (contact.Id <= 0 || _repository.GetById(contact.Id) == null)
                return false;

            _repository.Update(contact);
            return true;
        }

        public bool DeleteContact(int id)
        {
            if (_repository.GetById(id) == null)
                return false;

            _repository.Delete(id);
            return true;
        }
    }
}
