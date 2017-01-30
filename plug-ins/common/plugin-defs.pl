%plugins = (
    'align-layers' => { ui => 1 },
    'file-compressor' => { gio => 1, libdep => 'Z:BZIP2:LZMA', cflags => 'LZMA_CFLAGS' },
    'file-dicom' => { ui => 1, gegl => 1, cflags => '-fno-strict-aliasing' },
    'file-gbr' => { ui => 1, gegl => 1 },
    'file-gegl' => { ui => 1, gegl => 1 },
    'file-gih' => { ui => 1, gegl => 1 },
    'file-glob' => {},
    'file-jp2-load' => { ui => 1, optional => 1, gegl => 1, libs => 'JP2_LIBS' },
    'file-pat' => { ui => 1, gegl => 1 },
    'file-png' => { ui => 1, gegl => 1, libs => 'PNG_LIBS', cflags => 'PNG_CFLAGS' },
    'file-pdf-load' => { ui => 1, optional => 1, libs => 'POPPLER_LIBS', cflags => 'POPPLER_CFLAGS' },
    'file-pdf-save' => { ui => 1, gegl => 1, optional => 1, libs => 'CAIRO_PDF_LIBS', cflags => 'CAIRO_PDF_CFLAGS' },
    'unit-editor' => { ui => 1 },
    'wavelet-decompose' => { ui => 1, gegl => 1 }
);
