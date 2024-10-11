# Cypercine(WIP)

Command Line Interface for Student Search of IITK.
The functionality is intentionally minimalistic adhering to unix philosophy.
Use pipes and other tools to extend usage, such as grep, wc etc.

### Building
```sh
$ make
```


### Installing
```sh
# make install
```

### Usage
```sh
sscli --help
```

```
USAGE:
   sscli [OPTIONS]

DESCRIPTION:
   Student Search CLI for IITK

OPTIONS:
   -h, -help             show usage
   -u, -update           update jsondb
   -c, -csv              enable csv mode
   -n, -name             search by name
   -l, -hall             search by hall
   -d, -dept             search by dept
   -r, -roll             search by roll
   -p, -prog             search by program
   -o, -home             search by hometown
   -y, -year             search by year
```
