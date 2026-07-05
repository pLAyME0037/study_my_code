using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Coupling
{
    public interface IContactService
    {
        IEnumerable<Contact> GetAllContacts();
        Contact GetContact(int id);
        bool CreateContact(Contact contact);
        bool UpdateContact(Contact contact);
        bool DeleteContact(int id);
    }
}
