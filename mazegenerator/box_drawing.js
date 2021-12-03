const l = 5;

function b(char, ...args) {
	const hex = ('0000'+char.charCodeAt(0).toString(16)).slice(-4);
	var index = 0;
	for (var i = 0; i < l; i++) {
		index *= 2;
		if (args[i]) {
			index += 1;
		}
	}
	if (chars[index]) {
		console.error(`Warning: index ${index} (${args.join('')}) already exists`);
	}
	chars[index] = '\\u' + hex;
}

const chars = Array(2 ** l).fill();

// args: up, right, down, left, bold

b('·', 0, 0, 0, 0, 0);
b('╵', 1, 0, 0, 0, 0);
b('╶', 0, 1, 0, 0, 0);
b('╷', 0, 0, 1, 0, 0);
b('╴', 0, 0, 0, 1, 0);
b('└', 1, 1, 0, 0, 0);
b('┘', 1, 0, 0, 1, 0);
b('┐', 0, 0, 1, 1, 0);
b('┌', 0, 1, 1, 0, 0);
b('│', 1, 0, 1, 0, 0);
b('─', 0, 1, 0, 1, 0);
b('├', 1, 1, 1, 0, 0);
b('┤', 1, 0, 1, 1, 0);
b('┬', 0, 1, 1, 1, 0);
b('┴', 1, 1, 0, 1, 0);
b('┼', 1, 1, 1, 1, 0);

b('•', 0, 0, 0, 0, 1);
b('━', 0, 1, 0, 1, 1);
b('┃', 1, 0, 1, 0, 1);
b('┏', 0, 1, 1, 0, 1);
b('┓', 0, 0, 1, 1, 1);
b('┗', 1, 1, 0, 0, 1);
b('┛', 1, 0, 0, 1, 1);
b('┣', 1, 1, 1, 0, 1);
b('┫', 1, 0, 1, 1, 1);
b('┳', 0, 1, 1, 1, 1);
b('┻', 1, 1, 0, 1, 1);
b('╋', 1, 1, 1, 1, 1);
b('╸', 0, 0, 0, 1, 1);
b('╹', 1, 0, 0, 0, 1);
b('╺', 0, 1, 0, 0, 1);
b('╻', 0, 0, 1, 0, 1);

chars.forEach((x, i) => {
	if (!x) {
		console.error('Warning: unused index ' + i);
	}
});

const p = 4;

var a = 'const std::string box_draw =';
chars.forEach((x, i) => {
	if (i % p == 0) {
		a += '\n\t"';
	}
	a += x;
	if ((i + 1) % p == 0) {
		a += '"';
	}
});

console.log(a + ';');
