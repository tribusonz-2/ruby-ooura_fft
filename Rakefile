# frozen_string_literal: true

require "bundler/gem_tasks"
require "minitest/test_task"
require "rdoc/task"

Minitest::TestTask.create

require "rake/extensiontask"
Rake::ExtensionTask.new("ooura_fft") do |ext|
  ext.lib_dir = "lib/ooura_fft"
end

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_files.include("lib/**/*.rb", "ext/**/*.c")
end

task default: :test
