size = 42
tab = {}

for i=1,size do
	tab[i] = {}
	for j=1,size do
		tab[i][j] = 0
	end
end

for i=1,size do
	for j=1,size do
		if (j > i) and (j > -i + size) then
			io.write(tab[i][j])
		else
			io.write(' ')
		end
	end
	io.write('\n')
end
