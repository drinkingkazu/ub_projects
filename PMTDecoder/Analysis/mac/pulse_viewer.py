import ROOT,sys

ROOT.gSystem.Load("libAnalysis")

from ROOT import ana_processor, PMT, pulse_viewer, storage_manager,std

proc=ana_processor()

proc.set_ana_output_file("ana.root")

proc.set_io_mode(storage_manager.READ)

proc.set_data_to_read(PMT.PULSE_COLLECTION)

proc.add_input_file(sys.argv[1])

my_ana=pulse_viewer()

proc.add_process(my_ana)

while 1:
    
    proc.process_event()
    if not proc.get_process_status() == proc.PROCESSING:
        break
        
    # get list of channels that got a pulse
    ch = my_ana.next_channel()

    while not ch==PMT.INVALID_CH:
        
        for x in xrange(my_ana.get_npulse(ch)):
            print "pulse %d/%d" % (x+1,my_ana.get_npulse(ch))
            h=my_ana.next_pulse(ch)

            options=['n','s','q']
            user_input=''

            while not user_input in options:

                if user_input:
                    print
                    print "\033[91m" + 'Invalid option: \"%s\"' % user_input + "\033[0m"
                
                print
                sys.stdout.write("\033[95m" + "Options:" + "\033[0m")
                sys.stdout.write(" ... \"s\" to save image, \"n\" for next pulse, \"q\" to exit.\n")
                sys.stdout.flush()
                print 
                sys.stdout.write("\033[93m" + 'Input [s/n/q]: ' + "\033[0m")
                sys.stdout.flush()
                user_input=sys.stdin.readline().rstrip('\n')

                if user_input=='q':
                    terminate=True
                    sys.exit(1)
                if user_input=='n':
                    break;
                if user_input=='s':
                    out_name='pulse_image.gif'
                    my_ana.get_canvas().SaveAs("%s+100" % out_name)
                    print 'Image saved in %s ... ' % out_name
                    user_input=''

        ch=my_ana.next_channel()

