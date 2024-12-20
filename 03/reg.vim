" Join all lines
:%j

"convert all mul(X,Y) to (X*Y)+ and get rid of the rest
:%s/.\{-}mul(\(\d\+\),\(\d\+\))/(\1*\2)+/g

" Remove last bit that regex didn't get
$F+D

" Yank the current line (the math expression)
:yank

" Open a new line in insert mode (a place to put the answer)
o

" Put the yanked formula into vim's built-in math expression solver
<C-R>=<C-R>"<CR><Esc>

" Copy the answer to the clipboard (then paste it into the webpage)
:yank+"
