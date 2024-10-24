# frozen_string_literal: true

require_relative "lib/ooura_fft/version"

Gem::Specification.new do |spec|
  spec.name = "ooura_fft"
  spec.version = OouraFFT::VERSION
  spec.authors = ["tribusonz"]
  spec.email = ["xwisepp62543@gmail.com"]

  spec.summary = "Front-end of OouraFFT for Ruby"
  spec.description = "Front-end of OouraFFT for Ruby."
  spec.homepage = "https://github.com/tribusonz-2/ruby-ooura_fft"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 3.0.0"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage
  spec.metadata["changelog_uri"] = "https://github.com/tribusonz-2/ruby-ooura_fft/changelog.md"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  gemspec = File.basename(__FILE__)
  spec.files = IO.popen(%w[git ls-files -z], chdir: __dir__, err: IO::NULL) do |ls|
    ls.readlines("\x0", chomp: true).reject do |f|
      (f == gemspec) ||
        f.start_with?(*%w[bin/ test/ spec/ features/ .git .github appveyor Gemfile])
    end
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions = %w{ext/ooura_fft/extconf.rb}

  spec.add_development_dependency "bundler"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"

  # For more information and examples about making a new gem, check out our
  # guide at: https://bundler.io/guides/creating_gem.html
end
