package Apache::Cookie;

use strict;
use mod_perl 1.17_01;
use Apache::Table ();

{
    no strict;
    $VERSION = '1.0';
    __PACKAGE__->mod_perl::boot($VERSION);
}

1;
__END__

=head1 NAME

Apache::Cookie - HTTP Cookies Class

=head1 SYNOPSIS

    use Apache::Cookie ();
    my $r = Apache->request;
    my $cookie = Apache::Cookie->new($r, ...);

=head1 DESCRIPTION

The Apache::Cookie module is a Perl interface to the cookie routines
in I<libapreq>.  The interface is based on Lincoln Stein's CGI::Cookie
module.

=head1 METHODS

I<Apache::Cookie> does not export any symbols to the caller's namespace.
Except for the request object passed to C<Apache::Cookie::new>, the OO
interface is identical to I<CGI::Cookie>.  Please consult the L<CGI::Cookie>
documentation for more details.

=over 4

=head2 new

Just like CGI::Cookie::new, but requires an I<Apache> request object:

        my $cookie = Apache::Cookie->new($r,
                             -name    =>  'foo', 
                             -value   =>  'bar', 
                             -expires =>  '+3M', 
                             -domain  =>  '.capricorn.com', 
                             -path    =>  '/cgi-bin/database',
                             -secure  =>  1 
                            ); 

=head2 bake

Put cookie in the oven to bake. 
(Add a I<Set-Cookie> header to the outgoing headers table.) 

    $cookie->bake;

=head2 parse

This method parses the given string if present, otherwise, the incoming 
I<Cookie> header:

    my $cookies = $cookie->parse; #hash ref

    my %cookies = $cookie->parse;

    my %cookies = $cookie->parse($cookie_string);

=head2 fetch

Fetch and parse the incoming I<Cookie> header:

    my $cookies = Apache::Cookie->fetch; #hash ref

    my %cookies = Apache::Cookie->fetch;

=head2 as_string

Format the cookie object as a string:

 #same as $cookie->bake
 $r->err_headers_out->add("Set-Cookie" => $cookie->as_string);

=head2 name

Get or set the name of the cookie:

 my $name = $cookie->name;

 $cookie->name("Foo");

=head2 value

Get or set the values of the cookie:

 my $value = $cookie->value;
 my @values = $cookie->value;

 $cookie->value("string");
 $cookie->value(\@array);

=head2 domain

Get or set the domain for the cookie:

 my $domain = $cookie->domain;
 $cookie->domain(".cp.net");

=head2 path

Get or set the path for the cookie:

 my $path = $cookie->path;
 $cookie->path("/");

=head2 expires

Get or set the expire time for the cookie:

 my $expires = $cookie->expires;
 $cookie->expires("+3h");

=head2 secure

Get or set the secure flag for the cookie:

 my $secure = $cookie->secure;
 $cookie->secure(1);

=back

=head1 BUGS

=over 4

=item RFC 2964-5 are not fully implemented.

=item C<value> should also accept a hash ref as argument.

=item Reportedly does not run (linking problem?) on Apple's OSX

=back

=head1 SEE ALSO

Apache(3), Apache::Request(3), CGI::Cookie(3)

=head1 AUTHOR

Doug MacEachern, updated for v1.0 by Joe Schaefer
