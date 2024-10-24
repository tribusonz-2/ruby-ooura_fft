# frozen_string_literal: true

require "test_helper"

class TestOouraFFT < Minitest::Test
  def sinewave(amp, f0, fs, n)
    amp * Math.sin(2 * Math::PI * f0 * n / fs)
  end

  def test_that_it_has_a_version_number
    refute_nil ::OouraFFT::VERSION
  end

  def test_cdft_is_all_complex
    assert OouraFFT.cdft(Array.new(8){rand}).all?(Complex)
  end

  def test_rdft_is_all_real
    assert OouraFFT.rdft(Array.new(8){rand}).all?(Float)
  end

  def test_ddct_is_cosine_transform
    nuseq = OouraFFT.ddct(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
    approx_zero = Range.new(-Float::EPSILON, Float::EPSILON)
    assert approx_zero.cover?(nuseq.sum)
  end

  def test_ddst_is_sine_transform
    nuseq = OouraFFT.ddst(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
    approx_one = Range.new(1-Float::EPSILON, 1+Float::EPSILON)
    assert approx_one.cover?(nuseq.sum)
  end
end
