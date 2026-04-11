
all:
	@make --no-print-directory -C src all

.PHONY: clean

clean:
	@make --no-print-directory -C src clean
	$(RM) Minesweeper
