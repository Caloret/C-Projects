vim.opt.number = true
vim.opt.mouse = 'a'
vim.opt.smartcase = true
vim.opt.hlsearch = false
vim.opt.wrap = true
vim.opt.breakindent = true
vim.opt.tabstop = 4
vim.opt.shiftwidth = 4
vim.opt.expandtab = true
vim.opt.backup = false
vim.g.mapleader = ' '

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
})

vim.opt.termguicolors = true
vim.cmd.colorscheme('monokai_ristretto')
require('lualine').setup({
    options = {
        icons_enabled = false,
        section_separators = '',
        component_separators = ''
    }
})

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

-- CUSTOM COMMANDS
--
vim.api.nvim_create_user_command('ReloadConfig', 'source $MYVIMRC', {})
