# AstraOS Kernel

Ce dossier contient le code source du noyau d'AstraOS, organisé par responsabilité.

## Organisation

- `arch/` : code dépendant de l'architecture x86, notamment la GDT, l'IDT, l'ISR, le TSS, et les structures de registres.
- `core/` : code central du noyau, avec la routine `kernel_main`, le shell, la gestion du terminal, le clavier, les processus utilisateur, les appels système et la temporisation.
- `irq/` : gestion des interruptions matérielles, PIC, PIT et gestionnaire d'interruptions.
- `mm/` : gestion de la mémoire du noyau, incluant le heap et le paging.
- `sched/` : scheduler et tâches légères (task), utilisé pour basculer entre des fonctions périodiques.
- `sys/` : gestion des exceptions et des erreurs système (par exemple les faults).

## Fichiers importants

- `core/kernel.c` : point d'entrée du noyau (`kernel_main`).
- `arch/gdt.c` et `arch/gdt.asm` : configuration de la table des segments.
- `arch/idt.c` et `arch/isr.asm` : configuration de la table des interruptions et stubs d'interruption.
- `irq/pit.c` : initialisation du timer PIT et incrémentation de `timer_ticks`.
- `mm/paging.c` : activation du paging pour la mémoire.
- `core/shell.c` : shell simple qui reçoit les entrées clavier et affiche des commandes.

## Build

Le `Makefile` racine compile le noyau depuis `kernel/` en trouvant automatiquement les fichiers `*.c` et en assemblant les sources `*.asm` nécessaires.

Commandes utiles :

```bash
make
make run
```

## Notes

- Les fichiers sont organisés pour séparer la logique machine (`arch/`), la logique noyau (`core/`), les interruptions (`irq/`), la mémoire (`mm/`), le scheduling (`sched/`) et les erreurs (`sys/`).
- Pour ajouter du code, place les nouveaux fichiers selon cette logique et inclue le dossier approprié si nécessaire.
