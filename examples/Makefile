all:
	$(MAKE) -C basic
	$(MAKE) -C mock_square
	$(MAKE) -C no_implementation
	$(MAKE) -C mock_exit
	$(MAKE) -C mock_struct_member_in_assert
	$(MAKE) -C mock_struct_member_out_copy
	! $(MAKE) -C mock_api_examples
	! $(MAKE) -C failures

clean:
	$(MAKE) -C basic clean
	$(MAKE) -C mock_square clean
	$(MAKE) -C no_implementation clean
	$(MAKE) -C mock_exit clean
	$(MAKE) -C mock_struct_member_in_assert clean
	$(MAKE) -C mock_struct_member_out_copy clean
	$(MAKE) -C mock_api_examples clean
	$(MAKE) -C failures clean
