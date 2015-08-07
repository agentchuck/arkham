colorscheme slate
set cindent
set sw=2
set ch=2
set expandtab
set lines=45 columns=100

" Perforce stuff. Disable for now.
" runtime perforce/perforceutils.vim
" runtime perforce/perforcemenu.vim
" nnoremap @pfa       :!p4 add %<CR>:e<CR>
" nnoremap @pfe       :!p4 edit %<CR>:e<CR>
" nnoremap @pfd       :!p4 diff %<CR>

map <S-Insert> <MiddleMouse>
map! <S-Insert> <MiddleMouse>
map <F8> :make<CR>

syntax on
filetype indent plugin on

function! LoadCscope()
  " Skip searching if we're under /home/... as there's a problem with /home on
  " linuxdev03...? Can't do a search in there without hanging for a long time.
  let filedir = expand('%:p:h')
  if (filedir !~ "home")
    let db = findfile("cscope.out", ".;")
    if (!empty(db))
      let path = strpart(db, 0, match(db, "/cscope.out$"))
      set nocscopeverbose " suppress 'duplicate connection' error
      exe "cs add " . db . " " . path
      set cscopeverbose
    endif
  endif
endfunction
au BufEnter /* call LoadCscope()
