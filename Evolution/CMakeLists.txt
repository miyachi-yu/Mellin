add_library( Evolution
  KernelBase.cc
  KernelPdfLib.cc
  Kernel.cc
  PDFx.cc
  PDF.cc
  DGLAPMatrix.cc
  DGLAPpdf.cc
  AnomalousDimension.cc
  KernelSum.cc
  KernelDiff.cc )

install( TARGETS Evolution DESTINATION lib )
