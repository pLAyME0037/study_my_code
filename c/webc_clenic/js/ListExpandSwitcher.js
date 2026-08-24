window.masterDetailController = {
    init() {
        document.querySelectorAll('.expand-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const masterId = e.currentTarget.dataset.masterId;
                const panel    = document.querySelector('.detail-panel-row[data-master-id="' + masterId + '"]');
                const expanded = e.currentTarget.getAttribute('aria-expanded') === 'true';
                e.currentTarget.setAttribute('aria-expanded', !expanded);
                e.currentTarget.textContent    = expanded ? '▼ Detail' : '▲ Detail';
                if (panel) panel.style.display = expanded ? 'none' : 'table-row';
            });
        });
        document.querySelectorAll('.tab-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const masterId = e.currentTarget.dataset.masterId;
                const tab = e.currentTarget.dataset.tab;
                document.querySelectorAll('.tab-btn[data-master-id="' + masterId + '"]').forEach(b => {
                    b.classList.remove('text-indigo-600', 'border-b-2', 'border-indigo-600');
                    b.classList.add('text-gray-500', 'hover:text-gray-700', 'dark:text-gray-400', 'dark:hover:text-gray-200');
                });
                e.currentTarget.classList.add('text-indigo-600', 'border-b-2', 'border-indigo-600');
                e.currentTarget.classList.remove('text-gray-500', 'hover:text-gray-700', 'dark:text-gray-400', 'dark:hover:text-gray-200');
                document.querySelectorAll('.tab-content[data-master-id="' + masterId + '"]').forEach(c => {
                    c.classList.toggle('hidden', c.dataset.tab !== tab);
                });
            });
        });
        document.querySelectorAll('.add-child-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const table = e.currentTarget.dataset.childTable;
                const masterId = e.currentTarget.dataset.masterId;
                window.location.href = '/' + table + '/create?master_id=' + masterId;
            });
        });
    }
};

document.addEventListener('DOMContentLoaded', () => window.masterDetailController.init());
