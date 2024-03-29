# $Id: NodeList.pm,v 1.2 2002/03/09 14:19:52 phish Exp $

package XML::LibXML::NodeList;
use strict;
use XML::LibXML::Boolean;
use XML::LibXML::Literal;
use XML::LibXML::Number;

use overload 
		'""' => \&to_literal,
                'bool' => \&to_boolean,
        ;

sub new {
	my $class = shift;
	bless [@_], $class;
}

sub pop {
	my $self = CORE::shift;
	CORE::pop @$self;
}

sub push {
	my $self = CORE::shift;
	CORE::push @$self, @_;
}

sub append {
	my $self = CORE::shift;
	my ($nodelist) = @_;
	CORE::push @$self, $nodelist->get_nodelist;
}

sub shift {
	my $self = CORE::shift;
	CORE::shift @$self;
}

sub unshift {
	my $self = CORE::shift;
	CORE::unshift @$self, @_;
}

sub prepend {
	my $self = CORE::shift;
	my ($nodelist) = @_;
	CORE::unshift @$self, $nodelist->get_nodelist;
}

sub size {
	my $self = CORE::shift;
	scalar @$self;
}

sub get_node { # uses array index starting at 1, not 0
	my $self = CORE::shift;
	my ($pos) = @_;
	$self->[$pos - 1];
}

*item = \&get_node;

sub get_nodelist {
	my $self = CORE::shift;
	@$self;
}

sub to_boolean {
	my $self = CORE::shift;
	return (@$self > 0) ? XML::LibXML::Boolean->True : XML::LibXML::Boolean->False;
}

# string-value of a nodelist is the string-value of the first node
sub string_value {
	my $self = CORE::shift;
	return '' unless @$self;
	return $self->[0]->string_value;
}

sub to_literal {
	my $self = CORE::shift;
	return XML::LibXML::Literal->new(
			join('', grep {defined $_} map { $_->string_value } @$self)
			);
}

sub to_number {
	my $self = CORE::shift;
	return XML::LibXML::Number->new(
			$self->to_literal
			);
}

1;
__END__

=head1 NAME

XML::LibXML::NodeList - a list of XML document nodes

=head1 DESCRIPTION

An XML::LibXML::NodeList object contains an ordered list of nodes, as
detailed by the W3C DOM documentation of Node Lists.

=head1 SYNOPSIS

  my $results = $dom->findnodes('//somepath');
  foreach my $context ($results->get_nodelist) {
    my $newresults = $context->findnodes('./other/element');
    ...
  }

=head1 API

=head2 new()

You will almost never have to create a new NodeSet object, as it is all
done for you by XPath.

=head2 get_nodelist()

Returns a list of nodes, the contents of the node list, as a perl list.

=head2 string_value()

Returns the string-value of the first node in the list.
See the XPath specification for what "string-value" means.

=head2 to_literal()

Returns the concatenation of all the string-values of all
the nodes in the list.

=head2 get_node($pos)

Returns the node at $pos. The node position in XPath is based at 1, not 0.

=head2 size()

Returns the number of nodes in the NodeSet.

=head2 pop()

Equivalent to perl's pop function.

=head2 push(@nodes)

Equivalent to perl's push function.

=head2 append($nodelist)

Given a nodelist, appends the list of nodes in $nodelist to the end of the
current list.

=head2 shift()

Equivalent to perl's shift function.

=head2 unshift(@nodes)

Equivalent to perl's unshift function.

=head2 prepend($nodeset)

Given a nodelist, prepends the list of nodes in $nodelist to the front of
the current list.

=cut
