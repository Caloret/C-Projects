vim.opt.number = true
vim.opt.mouse = 'a'
vim.opt.smartcase = true
vim.opt.hlsearch = false
vim.opt.wrap = false
vim.opt.breakindent = true
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.backup = false
vim.g.mapleader = ' '
vim.opt.termguicolors = true

-- CUSTOM KEYBINDINGS
--
vim.keymap.set({'n', 'x'}, '<leader>c', '"+y')
vim.keymap.set({'n', 'x'}, '<leader>v', '"+p')
vim.keymap.set({'n', 'x'}, 'x', '"_x')
vim.keymap.set('n', '<leader>a', ':keepjumps normal! ggVG<CR>')
vim.keymap.set('n', '<F3>', '<cmd>Lexplore<cr>')
vim.keymap.set('v', '<c-f>', 'y<ESC>/<c-r>"<CR>')
vim.keymap.set('v', '<c-r>', '"hy:%s/<C-r>h//g<left><left>')
vim.keymap.set('v', '<leader>f', 'y<ESC>:bufdo /<c-r>"<CR>')
vim.keymap.set('v', '<leader>r', '"hy:bufdo %s/<C-r>h//g<left><left>')
vim.keymap.set('n', '<leader>o', 'o<ESC>')
vim.keymap.set('n', '<leader>O', 'O<ESC>')

--
-- Plugin Manager Installation
-- 
local lazy = {}

function lazy.install(path)
  if not vim.loop.fs_stat(path) then
    print('Installing lazy.nvim....')
    vim.fn.system({
      'git',
      'clone',
      '--filter=blob:none',
      'https://github.com/folke/lazy.nvim.git',
      '--branch=stable', -- latest stable release
      path,
    })
  end
end

function lazy.setup(plugins)
  -- Pueden comentar la siguiente línea una vez que lazy.nvim esté instalado
  -- lazy.install(lazy.path)

  vim.opt.rtp:prepend(lazy.path)
  require('lazy').setup(plugins, lazy.opts)
end

lazy.path = vim.fn.stdpath('data') .. '/lazy/lazy.nvim'
lazy.opts = {}

lazy.setup({
  ---
  -- Lista de plugins
  ---
    {'folke/tokyonight.nvim'},
    {'nvim-lualine/lualine.nvim'},
    {'joshdick/onedark.vim'},
    {'lunarvim/darkplus.nvim'},
    {'tanvirtin/monokai.nvim'},
    {'akinsho/bufferline.nvim'},
-- Before using markdown-preview, yarn and node-js must be installed in the system
-- Check https://classic.yarnpkg.com/lang/en/docs/install/#debian-rc for installation guide
-- After installing it run the command :call mkdp#util#install()
    {'iamcco/markdown-preview.nvim'},
})

vim.cmd.colorscheme('monokai_ristretto')
require('lualine').setup({
    options = {
        icons_enabled = false,
        section_separators = '',
        component_separators = ''
    }
})

-- This plugin allows opening each file as a tab by default instead of using buffers
-- Does not work properly in nvim-qt.exe
require('bufferline').setup({
  options = {
    mode = 'buffers',
    offsets = {
      {filetype = 'NvimTree'}
    },
  },
  highlights = {
    buffer_selected = {
      italic = false
    },
    indicator_selected = {
      fg = {attribute = 'fg', highlight = 'Function'},
      italic = false
    }
  }
})

-- set to 1, nvim will open the preview window after entering the markdown buffer
-- default: 0
vim.g['mkdp_auto_start'] = 0

-- set to 1, the nvim will auto close current preview window when change
-- from markdown buffer to another buffer
-- default: 1
vim.g['mkdp_auto_close'] = 1

-- set to 1, the vim will refresh markdown when save the buffer or
-- leave from insert mode, default 0 is auto refresh markdown as you edit or
-- move the cursor
-- default: 0
vim.g['mkdp_refresh_slow'] = 0

-- set to 1, the MarkdownPreview command can be use for all files,
-- by default it can be use in markdown file
-- default: 0
vim.g['mkdp_command_for_global'] = 0

-- set to 1, preview server available to others in your network
-- by default, the server listens on localhost (127.0.0.1)
-- default: 0
vim.g['mkdp_open_to_the_world'] = 0

-- use custom IP to open preview page
-- useful when you work in remote vim and preview on local browser
-- more detail see: https://github.com/iamcco/markdown-preview.nvim/pull/9
-- default empty
vim.g['mkdp_open_ip'] = ''

-- specify browser to open preview page
-- for path with space
-- valid: `/path/with\ space/xxx`
-- invalid: `/path/with\\ space/xxx`
-- default: ''
-- TO CHANGE FROM WINDOWS/LINUX
vim.g['mkdp_browser'] = '/usr/bin/firefox'

-- set to 1, echo preview page url in command line when open preview page
-- default is 0
vim.g['mkdp_echo_preview_url'] = 1

-- a custom vim function name to open preview page
-- this function will receive url as param
-- default is empty
vim.g['mkdp_browserfunc'] = ''

-- options for markdown render
-- mkit: markdown-it options for render
-- katex: katex options for math
-- uml: markdown-it-plantuml options
-- maid: mermaid options
-- disable_sync_scroll: if disable sync scroll, default 0
-- sync_scroll_type: 'middle', 'top' or 'relative', default value is 'middle'
--   middle: mean the cursor position alway show at the middle of the preview page
--   top: mean the vim top viewport alway show at the top of the preview page
--   relative: mean the cursor position alway show at the relative positon of the preview page
-- hide_yaml_meta: if hide yaml metadata, default is 1
-- sequence_diagrams: js-sequence-diagrams options
-- content_editable: if enable content editable for preview page, default: v:false
-- disable_filename: if disable filename header for preview page, default: 0
vim.g['mkdp_preview_options'] = {
     mkit = {},
     katex = {},
     uml = {},
     maid = {},
     disable_sync_scroll = 0,
     sync_scroll_type = 'middle',
     hide_yaml_meta = 1,
     sequence_diagrams = {},
     flowchart_diagrams = {},
     content_editable = false,
     disable_filename = 0,
     toc = {}
 }

-- use a custom markdown style must be absolute path
-- like '/Users/username/markdown.css' or expand('~/markdown.css')
vim.g['mkdp_markdown_css'] = ''

-- use a custom highlight style must absolute path
-- like '/Users/username/highlight.css' or expand('~/highlight.css')
vim.g['mkdp_highlight_css'] = ''

-- use a custom port to start server or empty for random
vim.g['mkdp_port'] = ''

-- preview page title
-- ${name} will be replace with the file name
-- vim.g['mkdp_page_title'] = '「${name}」'
-- vim.g['mkdp_page_title'] = '${name}'

-- recognized filetypes
-- these filetypes will have MarkdownPreview... commands
vim.g['mkdp_filetypes'] = {'markdown'}

-- set default theme (dark or light)
-- By default the theme is define according to the preferences of the system
vim.g['mkdp_theme'] = 'dark'

-- CUSTOM COMMANDS
--
vim.api.nvim_create_user_command('ReloadConfig', 'source $MYVIMRC', {})
