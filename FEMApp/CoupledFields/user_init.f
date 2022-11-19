c     program test              
c     character*10 inpf,outf    
c     data inpf/'elmt14.txt'/,outf/'outf.txt'/
c                              ! Program testowy skonentowany działa. 
c     call user_init(inpf,outf)! Na kartotece musisz mieć includy,  
c     write(*,*) inpf,outf     ! albo podać link do kompilacji, np.
c                   !gfortran user_init.f -I ../../feaps/feap81/include/
c     end


      subroutine user_init(inpf,outf)

      include 'bdata.h'
      include 'cdata.h'
      include 'eldata.h'
      include 'iofile.h'
      include 'ptdat6.h'
      include 'strnum.h'
      include 'eltran.h'
      character*10 inpf,outf

c      write(*,*) inpf,outf
      ior = 15
      iow = 16
      nel = 27
      open(ior, file = inpf, status = "old")
      open(iow, file = outf, status = "unknown")

      end
      
