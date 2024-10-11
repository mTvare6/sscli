include config.mk
BIN=sscli
TUI=sstui


all:
	$(CC) src/main.c src/jsonhandler.c -lcurl -ljson-c -Iincludes -o $(BIN)


install:
	@cp $(BIN) $(DESTDIR)$(PREFIX)/bin/$(BIN)
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/$(BIN)
	@cp $(TUI) $(DESTDIR)$(PREFIX)/bin/$(TUI)
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/$(TUI)


uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(TUI)

